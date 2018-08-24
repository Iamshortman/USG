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

	void UpdateLinearVelocity(double delta_time);
	void UpdateAngularVelocity(double delta_time);

	vector3D thruster_torque = vector3D(400.0);
	double thruster_force[6] = { 20000.0, 20000.0, 20000.0, 20000.0, 60000.0, 30000.0 };
};


#endif // COMPONENT_SHIP_FLIGHT_HPP
