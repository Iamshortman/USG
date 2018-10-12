#ifndef SEAT_HPP
#define SEAT_HPP

#include "Common/EntityX_Include.hpp"
#include "Common/World/WorldSystem.hpp"
#include "Common/World/NodeSystem.hpp"

#include "Common/Logger/Logger.hpp"

#include <string>
using std::string;

struct Seat
{
	Seat(string name)
		:seat_name(name) {};
	const string seat_name;

	Entity seated_entity;
	bool hasSeatedEntity()
	{
		return seated_entity.id() != Entity::INVALID;
	};
};

struct SeatLink
{
	SeatLink(Entity parent) : parent_entity(parent) {}
	const Entity parent_entity;
};

struct SeatInteract
{
	SeatInteract(string name)
		: seat_name(name) {};

	string seat_name;
};


struct AddToSeatEvent
{
	AddToSeatEvent(Entity entity_to_add, Entity root_seat_entity, string seat_name)
		: entity_to_add(entity_to_add), root_seat_entity(root_seat_entity), seat_name(seat_name) {};
	Entity entity_to_add;
	Entity root_seat_entity;
	string seat_name;
};

struct RemoveFromSeatEvent
{
	RemoveFromSeatEvent(Entity entity_to_remove, WorldId destination_world, Transform destionation_transform)
		: entity_to_remove(entity_to_remove), destination_world(destination_world), destionation_transform(destionation_transform) {};
	Entity entity_to_remove;

	WorldId destination_world;
	Transform destionation_transform;
};

static Entity lookForSeat(Entity node, string seat_name)
{
	if (node.has_component<Seat>())
	{
		if (node.component<Seat>()->seat_name == seat_name)
		{
			return node;
		}
	}

	if (node.has_component<NodeHost>())
	{
		ComponentHandle<NodeHost> nodes = node.component<NodeHost>();

		for (auto child : nodes->child_list)
		{
			Entity temp = lookForSeat(child, seat_name);

			if (temp.valid())
			{
				return temp;
			}
		}
	}

	return Entity(); //Invalid Entity
};


class SeatSystem : public System<SeatSystem>, public Receiver<SeatSystem>
{
public:

	void update(EntityManager &es, EventManager &events, TimeDelta dt) override
	{
		//Only a system for the events, doesn't need to update anything
	};

	//Events
	void configure(entityx::EventManager &event_manager)
	{
		event_manager.subscribe<AddToSeatEvent>(*this);
		event_manager.subscribe<RemoveFromSeatEvent>(*this);
		event_manager.subscribe<ComponentRemovedEvent<Seat>>(*this);
	};

	void receive(const AddToSeatEvent& event)
	{
		Entity entity_to_seat = event.entity_to_add;
		if (entity_to_seat.has_component<SeatLink>())
		{
			return;
		}

		Entity seat_entity = lookForSeat(event.root_seat_entity, event.seat_name);

		if (seat_entity.has_component<Seat>())
		{
			ComponentHandle<Seat> seat = seat_entity.component<Seat>();
			if (!seat->hasSeatedEntity())
			{
				if (entity_to_seat.has_component<World>())
				{
					entity_to_seat.remove<World>();
				}

				if (entity_to_seat.has_component<Transform>())
				{
					entity_to_seat.remove<Transform>();
				}

				entity_to_seat.assign<SeatLink>(seat_entity);
				seat->seated_entity = entity_to_seat;
			}
			else
			{
				Logger::getInstance()->logDebug("Warning: Seat already filled\n");
			}
		}
		else
		{
			Logger::getInstance()->logError("Warning: Can't find seat: %s\n", event.seat_name);
		}
	};
	void receive(const RemoveFromSeatEvent& event)
	{
		Entity entity_to_remove = event.entity_to_remove;

		if (entity_to_remove.has_component<SeatLink>())
		{
			Entity seat = entity_to_remove.component<SeatLink>()->parent_entity;
			if (seat.has_component<Seat>())
			{
				seat.component<Seat>()->seated_entity = Entity();
			}

			entity_to_remove.remove<SeatLink>();
			entity_to_remove.assign<Transform>(event.destionation_transform);
			entity_to_remove.assign<World>(event.destination_world);
		}
	};

	void receive(const ComponentRemovedEvent<Seat>& event)
	{
		ComponentHandle<Seat> seat = event.component;
		//Kill the entity in the seat
		if (seat->hasSeatedEntity())
		{
			EntityDestroyer::destroyEntity(seat->seated_entity);
		}
	};

private:
};

#endif //SEAT_HPP