#ifndef NODE_SYSTEM_HPP
#define NODE_SYSTEM_HPP

#include "Common/EntityX_Include.hpp"
#include "Common/Transform.hpp"

#include <set>

struct Node
{
	Node(Entity parent, Transform transform = Transform()) : parent_entity(parent), local_transform(transform) {}

	const Entity parent_entity;
	Transform local_transform;
};

struct NodeHost
{
	std::set<Entity> child_list;
};

class NodeSystem : public System<NodeSystem>, public Receiver<NodeSystem>
{
public:
	void update(EntityManager &es, EventManager &events, TimeDelta dt) override;

	//Events
	void configure(entityx::EventManager &event_manager);

	void receive(const ComponentAddedEvent<Node> &event);
	void receive(const ComponentRemovedEvent<Node>& event);

	void receive(const ComponentAddedEvent<NodeHost> &event);
	void receive(const ComponentRemovedEvent<NodeHost>& event);

private:
};

#endif //NODE_SYSTEM_HPP