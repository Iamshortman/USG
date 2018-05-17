#ifndef COMPONENT_SHIP_FLIGHT_HPP
#define COMPONENT_SHIP_FLIGHT_HPP

#include "Common/Component.hpp"

class ComponentShipFlight : public Component
{
	ComponentShipFlight();
	virtual ~ComponentShipFlight();

	virtual void update(double delta_time);
};


#endif // COMPONENT_SHIP_FLIGHT_HPP
