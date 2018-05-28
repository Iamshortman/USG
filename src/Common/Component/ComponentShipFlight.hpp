#ifndef COMPONENT_SHIP_FLIGHT_HPP
#define COMPONENT_SHIP_FLIGHT_HPP

#include "Common/Component.hpp"

#include "Common/GLM_Include.hpp"
#include "Common/Rotation.hpp"


const vector3B Directions3D[] =
{
	vector3B(0.0, 0.0, 1.0),
	vector3B(0.0, 0.0, -1.0),
	vector3B(0.0, 1.0, 0.0),
	vector3B(0.0, -1.0, 0.0),
	vector3B(1.0, 0.0, 0.0),
	vector3B(-1.0, 0.0, 0.0),
};

class ComponentShipFlight : public Component
{
public:
	ComponentShipFlight();
	virtual ~ComponentShipFlight();

	virtual void update(double delta_time);

	vector3D linear_input;

	vector3D linear_max_speed = vector3D(50.0, 50.0, 200.0);
	vector3D linear_acceleration = vector3D(10.0, 10.0, 30.0);
	vector3D linear_braking_acceleration = vector3D(20.0, 20.0, 60.0);

	/*vector3D linear_max_speed = vector3D(50.0, 50.0, 200.0);
	vector3D linear_force = vector3D(50000.0, 50000.0, 150000.0);
	vector3D linear_drag_constant = vector3D(0.3, 0.3, 0.1);*/


	vector3D angular_input;

	vector3D max_angular_speed = vector3D(0.7);
	vector3D angular_acceleration = vector3D(1.0);
	vector3D angular_braking_acceleration = vector3D(3.0);
};


#endif // COMPONENT_SHIP_FLIGHT_HPP
