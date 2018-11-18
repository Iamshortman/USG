#include "VectorPidController.hpp"

VectorPidController::VectorPidController(double proptional, double intergral, double derivative, double min, double max, double i_max)
	:VectorPidController(vector3D(proptional), vector3D(intergral), vector3D(derivative), vector3D(min), vector3D(max), vector3D(i_max))
{
}

VectorPidController::VectorPidController(vector3D proptional, vector3D intergral, vector3D derivative, vector3D min, vector3D max, vector3D i_max)
	:pid_x(proptional.x, intergral.x, derivative.x, min.x, max.x, i_max.x),
	pid_y(proptional.y, intergral.y, derivative.y, min.y, max.y, i_max.y),
	pid_z(proptional.z, intergral.z, derivative.z, min.z, max.z, i_max.z)
{
}

vector3D VectorPidController::calculate(vector3D current, vector3D desired, vector3D delta, double delta_time)
{
	vector3D error = current - desired;
	return vector3D(pid_x.calculate(error.x, delta.x, delta_time), pid_y.calculate(error.y, delta.y, delta_time), pid_z.calculate(error.z, delta.z, delta_time));
}
