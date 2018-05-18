#ifndef COMPONENT_SHIP_FLIGHT_HPP
#define COMPONENT_SHIP_FLIGHT_HPP

#include "Common/Component.hpp"

#include "Common/GLM_Include.hpp"
#include "Common/Rotation.hpp"

class ComponentShipFlight : public Component
{
public:
	ComponentShipFlight();
	virtual ~ComponentShipFlight();

	virtual void update(double delta_time);

	vector3D thrust_input;

	double max_speed[6];
	double engine_acceleration[6];
	double braking_acceleration[6];


	double spaceMaxSpeed = 500.0;
	double spaceAcceleration = 20.0;

	double forward_thrust = 10;
	double reverse_thrust = 20;
	double breaking_force = 75;
};


#endif // COMPONENT_SHIP_FLIGHT_HPP
