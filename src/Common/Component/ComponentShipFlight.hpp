#ifndef COMPONENT_SHIP_FLIGHT_HPP
#define COMPONENT_SHIP_FLIGHT_HPP

#include "Common/Component/Component.hpp"

#include "Common/GLM_Include.hpp"
#include "Common/Rotation.hpp"

class ComponentShipFlight : public Component
{
public:
	ComponentShipFlight();
	virtual ~ComponentShipFlight();

	virtual void update(double delta_time);

	vector3D linear_input;

	double linear_max_speed[6] =	{ 50.0, 50.0, 50.0, 50.0, 500.0, 50.0 };
	double linear_acceleration[6] = { 2.0, 2.0, 2.0, 2.0, 5.0, 2.0 };
	double linear_braking_expo[6] = { 0.01, 0.01, 0.01, 0.01, 0.01, 0.01 };

	double linear_aero_drag[6] = { 0.2, 0.2, 0.2, 0.2, 0.01, 0.2 };

	vector3D angular_input;

	vector3D max_angular_speed = vector3D(0.7);
	vector3D angular_acceleration = vector3D(1.0);
	vector3D angular_braking_acceleration = vector3D(3.0);
};


#endif // COMPONENT_SHIP_FLIGHT_HPP
