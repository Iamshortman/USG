#include "QuaternionPidController.hpp"

quaternionD deltaOrientation(quaternionD from, quaternionD to)
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

quaternionD subtract(quaternionD a, quaternionD b)
{
	return quaternionD(a.w - b.w, a.x - b.x, a.y - b.y, a.z - b.z);
}

QuaternionPidController::QuaternionPidController(double proptional, double intergral, double derivative, double min, double max, double i_max)
	:pid_w(proptional, intergral, derivative, min, max, i_max),
	pid_x(proptional, intergral, derivative, min, max, i_max),
	pid_y(proptional, intergral, derivative, min, max, i_max),
	pid_z(proptional, intergral, derivative, min, max, i_max)
{

}

vector3D QuaternionPidController::calculate(quaternionD current_orientation, quaternionD desired_orientation, vector3D angular_velocity, double delta_time)
{
	quaternionD delta_orientation = deltaOrientation(current_orientation, desired_orientation);
	//printf("Required: %s\n", glm::to_string(required_rotation).c_str());

	quaternionD identity(1.0, 0.0, 0.0, 0.0);

	quaternionD result = subtract(identity, delta_orientation);//calculatePidQuaternion(error, delta, delta_time);
	//printf("result: %s\n", glm::to_string(result).c_str());

	matrix4D orthogonalize_matrix;
	orthogonalize_matrix[0][0] = -delta_orientation.x * -delta_orientation.x + -delta_orientation.y * -delta_orientation.y + -delta_orientation.z * -delta_orientation.z;
	orthogonalize_matrix[0][1] = -delta_orientation.x * delta_orientation.w + -delta_orientation.y * -delta_orientation.z + -delta_orientation.z * delta_orientation.y;
	orthogonalize_matrix[0][2] = -delta_orientation.x * delta_orientation.z + -delta_orientation.y * delta_orientation.w + -delta_orientation.z * -delta_orientation.x;
	orthogonalize_matrix[0][3] = -delta_orientation.x * -delta_orientation.y + -delta_orientation.y * delta_orientation.x + -delta_orientation.z * delta_orientation.w;
	orthogonalize_matrix[1][0] = delta_orientation.w * -delta_orientation.x + -delta_orientation.z * -delta_orientation.y + delta_orientation.y * -delta_orientation.z;
	orthogonalize_matrix[1][1] = delta_orientation.w * delta_orientation.w + -delta_orientation.z * -delta_orientation.z + delta_orientation.y * delta_orientation.y;
	orthogonalize_matrix[1][2] = delta_orientation.w * delta_orientation.z + -delta_orientation.z * delta_orientation.w + delta_orientation.y * -delta_orientation.x;
	orthogonalize_matrix[1][3] = delta_orientation.w * -delta_orientation.y + -delta_orientation.z * delta_orientation.x + delta_orientation.y * delta_orientation.w;
	orthogonalize_matrix[2][0] = delta_orientation.z * -delta_orientation.x + delta_orientation.w * -delta_orientation.y + -delta_orientation.x * -delta_orientation.z;
	orthogonalize_matrix[2][1] = delta_orientation.z * delta_orientation.w + delta_orientation.w * -delta_orientation.z + -delta_orientation.x * delta_orientation.y;
	orthogonalize_matrix[2][2] = delta_orientation.z * delta_orientation.z + delta_orientation.w * delta_orientation.w + -delta_orientation.x * -delta_orientation.x;
	orthogonalize_matrix[2][3] = delta_orientation.z * -delta_orientation.y + delta_orientation.w * delta_orientation.x + -delta_orientation.x * delta_orientation.w;
	orthogonalize_matrix[3][0] = -delta_orientation.y * -delta_orientation.x + delta_orientation.x * -delta_orientation.y + delta_orientation.w * -delta_orientation.z;
	orthogonalize_matrix[3][1] = -delta_orientation.y * delta_orientation.w + delta_orientation.x * -delta_orientation.z + delta_orientation.w * delta_orientation.y;
	orthogonalize_matrix[3][2] = -delta_orientation.y * delta_orientation.z + delta_orientation.x * delta_orientation.w + delta_orientation.w * -delta_orientation.x;
	orthogonalize_matrix[3][3] = -delta_orientation.y * -delta_orientation.y + delta_orientation.x * delta_orientation.x + delta_orientation.w * delta_orientation.w;

	vector4D vec4 = vector4D(result.w, result.x, result.y, result.z);
	vec4 = orthogonalize_matrix * vec4;
	result = quaternionD(vec4.x, vec4.y, vec4.z, vec4.w);
	//printf("result1: %s\n", glm::to_string(result).c_str());

	result = (result * -2.0) * glm::inverse(delta_orientation);
	//printf("result2: %s\n", glm::to_string(result).c_str());

	vector3D local_input = (glm::inverse(current_orientation) * vector3D(result.x, result.y, result.z)) / 2.0;
	vector3D local_velocity = glm::inverse(current_orientation) * angular_velocity;

	double x = this->pid_x.calculate(local_input.x, local_velocity.x, delta_time);
	double y = this->pid_y.calculate(local_input.y, local_velocity.y, delta_time);
	double z = -this->pid_z.calculate(local_input.z, local_velocity.z, delta_time);

	return vector3D(x, y, z);
}

quaternionD QuaternionPidController::calculatePidQuaternion(quaternionD error, quaternionD delta, double delta_time)
{
	double w = this->pid_w.calculate(error.w, delta.w, delta_time);
	double x = this->pid_x.calculate(error.x, delta.x, delta_time);
	double y = this->pid_y.calculate(error.y, delta.y, delta_time);
	double z = this->pid_z.calculate(error.z, delta.z, delta_time);

	return quaternionD
	(
		w,
		x,
		y,
		z
	);
}
