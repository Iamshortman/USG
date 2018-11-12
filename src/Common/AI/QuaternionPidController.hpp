#ifndef QUATERNION_PID_CONTROLLER_HPP
#define QUATERNION_PID_CONTROLLER_HPP

#include "Common/AI/PidController.hpp"
#include "Common/GLM_Include.hpp"

class QuaternionPidController
{
public:
	QuaternionPidController(double proptional, double intergral, double derivative, double min, double max, double i_max);

	vector3D calculate(quaternionD current_orientation, quaternionD desired_orientation, vector3D angular_velocity, double delta_time);

private:
	quaternionD calculatePidQuaternion(quaternionD error, double delta_time);
	quaternionD RequiredRotation(quaternionD from, quaternionD to);
	quaternionD subtract(quaternionD a, quaternionD b);

	PidController pid_w;
	PidController pid_x;
	PidController pid_y;
	PidController pid_z;
};

#endif //QUATERNION_PID_CONTROLLER_HPP