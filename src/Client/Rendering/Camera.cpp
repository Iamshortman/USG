#include "Camera.hpp"
#include "Common/Entity/Entity.hpp"
#include "Common/Entity/Node.hpp"

Camera::Camera()
{
	this->frame_of_view = 90.0;
}

matrix4 Camera::getProjectionMatrix(int screenWidth, int screenHeight)
{
	float aspect_ratio = ((float)screenWidth) / ((float)screenHeight);

	float f = 1.0f / tan(glm::radians(this->frame_of_view) / 2.0f);
	return glm::mat4(
		f / aspect_ratio, 	0.0f, 	0.0f, 			0.0f,
		0.0f, 				f, 		0.0f,			0.0f,
		0.0f, 				0.0f, 	0.0f, 			-1.0f,
		0.0f, 				0.0f, 	this->z_near, 	0.0f);
}

matrix4 Camera::getProjectionMatrix(vector2I& screen_size)
{
	return this->getProjectionMatrix(screen_size.x, screen_size.y);
}

matrix4 Camera::getOrthographicMatrix(float x_bounds, float y_bounds)
{
	return glm::ortho(-x_bounds, x_bounds, -y_bounds, y_bounds, this->z_near, this->z_far);
}
