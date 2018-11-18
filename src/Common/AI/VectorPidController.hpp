#pragma once

#include "Common/AI/PidController.hpp"
#include "Common/GLM_Include.hpp"

class VectorPidController
{
public:
	VectorPidController(double proptional, double intergral, double derivative, double min, double max, double i_max);
	VectorPidController(vector3D proptional, vector3D intergral, vector3D derivative, vector3D min, vector3D max, vector3D i_max);

	vector3D calculate(vector3D current, vector3D desired, vector3D delta, double delta_time);

private:
	PidController pid_x;
	PidController pid_y;
	PidController pid_z;
};