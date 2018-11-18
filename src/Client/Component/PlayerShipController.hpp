#pragma once

#include "Common/GLM_Include.hpp"
#include "Common/Component/ComponentEntity.hpp"
#include "Common/AI/QuaternionPidController.hpp"
#include "Common/AI/VectorPidController.hpp"
enum RotationalFlightMode
{
	Normal,
	Retro_Burn,
};

class PlayerShipController : public ComponentEntity
{
public:
	PlayerShipController(Entity* entity);
	~PlayerShipController();

	virtual void update(double delta_time);

	vector3D player_input_angular;
	vector3D player_input_linear;
	bool retro_burn_mode_input = false;

private:
	RotationalFlightMode rotation_mode = Normal;

	QuaternionPidController retro_burn_controller;

	VectorPidController velocity_limit_angular;
	vector3D max_angular_speed = vector3D(M_PI, M_PI, M_PI / 2.0);
};

