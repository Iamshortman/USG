#ifndef COMPONENT_SHIP_FLIGHT_HPP
#define COMPONENT_SHIP_FLIGHT_HPP

#include "Common/Component/ComponentEntity.hpp"

#include "Common/GLM_Include.hpp"

enum Direction
{
	Leftward = 0, //X
	Rightward,
	Upward, //Y
	Downward,
	Forward, //Z
	Backward,
};

class ShipFlightController : public ComponentEntity
{
public:
	ShipFlightController(Entity* entity);
	virtual ~ShipFlightController();

	virtual void update(double delta_time);

	vector3D linear_input = vector3D(0.0);
	vector3D angular_input = vector3D(0.0);
	bool flight_assist = false;

	vector3D max_angular_speed = vector3D(0.6);
	vector3D angular_acceleration = vector3D(1.0);
	vector3D angular_braking_acceleration = vector3D(3.0);

	void UpdateLinearVelocity(double delta_time);
	void UpdateAngularVelocity(double delta_time);

	double thruster_force[6] = {6000.0, 6000.0, 6000.0, 6000.0, 60000.0, 20000.0 };
};


#endif // COMPONENT_SHIP_FLIGHT_HPP
