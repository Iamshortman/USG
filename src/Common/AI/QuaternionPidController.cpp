#include "QuaternionPidController.hpp"

QuaternionPidController::QuaternionPidController(double proptional, double intergral, double derivative, double min, double max, double i_max)
	:pid_x(proptional, intergral, derivative, min, max, i_max),
	pid_y(proptional, intergral, derivative, min, max, i_max),
	pid_z(proptional, intergral, derivative, min, max, i_max)
{

}

vector3D QuaternionPidController::calculate(quaternionD current_orientation, quaternionD desired_orientation, double delta_time)
{
	quaternionD error = this->RequiredRotation(current_orientation, desired_orientation);

	vector3D error_angles = glm::eulerAngles(error);
	printf("Eular: %lf\n", error_angles.y);

	return calculatePidQuaternion(error_angles, delta_time);
}

vector3D QuaternionPidController::calculatePidQuaternion(vector3D error, double delta_time)
{
	double x = this->pid_x.calculate(error.x, delta_time);
	double y = this->pid_y.calculate(error.y, delta_time);
	double z = this->pid_z.calculate(error.z, delta_time);

	return vector3D
	(
		x,
		y,
		z
	);
}

quaternionD QuaternionPidController::RequiredRotation(quaternionD from, quaternionD to)
{
	quaternionD required_rotation = to * glm::inverse(from);

	// Flip the sign if w is negative.
	// This makes sure we always rotate the shortest angle to match the desired rotation.
	if (required_rotation.w < 0.0f)
	{
		required_rotation.x *= -1.0f;
		required_rotation.y *= -1.0f;
		required_rotation.z *= -1.0f;
		required_rotation.w *= -1.0f;
	}

	return glm::normalize(required_rotation);
}

quaternionD QuaternionPidController::subtract(quaternionD a, quaternionD b)
{
	return quaternionD(a.w - b.w, a.x - b.x, a.y - b.y, a.z - b.z);
}
