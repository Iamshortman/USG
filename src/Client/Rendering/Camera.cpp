#include "Camera.hpp"

Camera::Camera()
{
	this->frame_of_view = 90.0;
}

matrix4 Camera::getProjectionMatrix(int screenWidth, int screenHeight)
{
	float aspect_ratio = ((float)screenWidth) / ((float)screenHeight);

	float fovy = 2 * atan(tan(this->frame_of_view / 2) / aspect_ratio);

	matrix4 projection_matrix = glm::tweakedInfinitePerspective(fovy, aspect_ratio, this->z_near);

	float f = 1.0f / tan(glm::radians(this->frame_of_view) / 2.0f);
	return glm::mat4(
		f / aspect_ratio, 0.0f, 0.0f, 0.0f,
		0.0f, f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, this->z_near, 0.0f);
}

matrix4 Camera::getProjectionMatrix(Window* window)
{
	int width, height;
	window->getWindowSize(width, height);
	return this->getProjectionMatrix(width, height);
}

matrix4 Camera::getOrthographicMatrix(float x_bounds, float y_bounds)
{
	return glm::ortho(-x_bounds, x_bounds, -y_bounds, y_bounds, this->z_near, this->z_far);
}
