#include "QuaternionPidController.hpp"

QuaternionPidController::QuaternionPidController(double proptional, double intergral, double derivative, double min, double max, double i_max)
	:pid_w(proptional, intergral, derivative, min, max, i_max),
	pid_x(proptional, intergral, derivative, min, max, i_max),
	pid_y(proptional, intergral, derivative, min, max, i_max),
	pid_z(proptional, intergral, derivative, min, max, i_max)
{

}

vector3D QuaternionPidController::calculate(quaternionD current_orientation, quaternionD desired_orientation, vector3D angular_velocity, double delta_time)
{
	quaternionD required_rotation = this->RequiredRotation(current_orientation, desired_orientation);
	//printf("Required: %s\n", glm::to_string(required_rotation).c_str());


	quaternionD identity(1.0, 0.0, 0.0, 0.0);

	quaternionD error = this->subtract(identity, required_rotation);
	//printf("error: %s\n", glm::to_string(error).c_str());


	quaternionD angular_velocity_quat(0.0, angular_velocity);
	//printf("angular_velocity_quat: %s\n", glm::to_string(angular_velocity_quat).c_str());


	quaternionD delta = angular_velocity_quat * required_rotation;
	//printf("delta: %s\n", glm::to_string(delta).c_str());


	quaternionD result = calculatePidQuaternion(error, delta_time);
	//printf("result: %s\n", glm::to_string(result).c_str());

	matrix4D orthogonalize_matrix;
	orthogonalize_matrix[0][0] = -required_rotation.x * -required_rotation.x + -required_rotation.y * -required_rotation.y + -required_rotation.z * -required_rotation.z;
	orthogonalize_matrix[0][1] = -required_rotation.x * required_rotation.w + -required_rotation.y * -required_rotation.z + -required_rotation.z * required_rotation.y;
	orthogonalize_matrix[0][2] = -required_rotation.x * required_rotation.z + -required_rotation.y * required_rotation.w + -required_rotation.z * -required_rotation.x;
	orthogonalize_matrix[0][3] = -required_rotation.x * -required_rotation.y + -required_rotation.y * required_rotation.x + -required_rotation.z * required_rotation.w;
	orthogonalize_matrix[1][0] = required_rotation.w * -required_rotation.x + -required_rotation.z * -required_rotation.y + required_rotation.y * -required_rotation.z;
	orthogonalize_matrix[1][1] = required_rotation.w * required_rotation.w + -required_rotation.z * -required_rotation.z + required_rotation.y * required_rotation.y;
	orthogonalize_matrix[1][2] = required_rotation.w * required_rotation.z + -required_rotation.z * required_rotation.w + required_rotation.y * -required_rotation.x;
	orthogonalize_matrix[1][3] = required_rotation.w * -required_rotation.y + -required_rotation.z * required_rotation.x + required_rotation.y * required_rotation.w;
	orthogonalize_matrix[2][0] = required_rotation.z * -required_rotation.x + required_rotation.w * -required_rotation.y + -required_rotation.x * -required_rotation.z;
	orthogonalize_matrix[2][1] = required_rotation.z * required_rotation.w + required_rotation.w * -required_rotation.z + -required_rotation.x * required_rotation.y;
	orthogonalize_matrix[2][2] = required_rotation.z * required_rotation.z + required_rotation.w * required_rotation.w + -required_rotation.x * -required_rotation.x;
	orthogonalize_matrix[2][3] = required_rotation.z * -required_rotation.y + required_rotation.w * required_rotation.x + -required_rotation.x * required_rotation.w;
	orthogonalize_matrix[3][0] = -required_rotation.y * -required_rotation.x + required_rotation.x * -required_rotation.y + required_rotation.w * -required_rotation.z;
	orthogonalize_matrix[3][1] = -required_rotation.y * required_rotation.w + required_rotation.x * -required_rotation.z + required_rotation.w * required_rotation.y;
	orthogonalize_matrix[3][2] = -required_rotation.y * required_rotation.z + required_rotation.x * required_rotation.w + required_rotation.w * -required_rotation.x;
	orthogonalize_matrix[3][3] = -required_rotation.y * -required_rotation.y + required_rotation.x * required_rotation.x + required_rotation.w * required_rotation.w;

	vector4D vec4 = vector4D(result.x, result.y, result.z, result.w);
	vec4 = orthogonalize_matrix * vec4;
	result = quaternionD(vec4.w, vec4.x, vec4.y, vec4.z);
	//printf("result1: %s\n", glm::to_string(result).c_str());

	quaternionD double_negative = result * -2.0;
	//printf("double_negative: %s\n", glm::to_string(double_negative).c_str());

	result = double_negative * glm::inverse(required_rotation);
	//printf("result2: %s\n", glm::to_string(result).c_str());

	return vector3D(result.x, result.y, result.z);
}

quaternionD QuaternionPidController::calculatePidQuaternion(quaternionD error, double delta_time)
{
	double w = this->pid_w.calculate(error.w, delta_time);
	double x = this->pid_x.calculate(error.x, delta_time);
	double y = this->pid_y.calculate(error.y, delta_time);
	double z = this->pid_z.calculate(error.z, delta_time);

	return quaternionD
	(
		w,
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
