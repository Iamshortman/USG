#ifndef VEHICLE_CONTROLLER_HPP
#define VEHICLE_CONTROLLER_HPP

#include "Common/EntityX_Include.hpp"
#include "Common/Component/Seat.hpp"

struct VehicleController
{
	std::set<Seat*> seats;
};


class VehicleSystem : public System<VehicleSystem>, public Receiver<VehicleSystem>
{
public:
	void update(EntityManager& es, EventManager& events, TimeDelta delta_time) override;

	//Events
	void configure(entityx::EventManager &event_manager);

	void receive(const ComponentAddedEvent<Seat> &event);
	void receive(const ComponentRemovedEvent<Seat>& event);
};

#endif //VEHICLE_CONTROLLER_HPP