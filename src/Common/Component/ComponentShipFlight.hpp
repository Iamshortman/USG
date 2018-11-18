#pragma once

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

	void setMassProps(double mass, matrix3 inv_world_tensor);

	virtual void update(double delta_time);

	vector3D linear_input = vector3D(0.0);
	vector3D angular_input = vector3D(0.0);
	bool flight_assist = false;

	void UpdateLinearVelocity(double delta_time);
	void UpdateAngularVelocity(double delta_time);

	vector3D thruster_torque = vector3D(16000.0);
	double thruster_force[6] = { 20000.0, 20000.0, 20000.0, 20000.0, 60000.0, 30000.0 };

	double getAngularAcceleration(int axis, double input);
	double getLinearAcceleration(int axis, double nput);
};
