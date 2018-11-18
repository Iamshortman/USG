#pragma once

#include "Common/AI/PidController.hpp"
#include "Common/GLM_Include.hpp"

class QuaternionPidController
{
public:
	QuaternionPidController(double proptional, double intergral, double derivative, double min, double max, double i_max);

	vector3D calculate(quaternionD current_orientation, quaternionD desired_orientation, vector3D angular_velocity, double delta_time);

private:
	PidController pid_x;
	PidController pid_y;
	PidController pid_z;
};