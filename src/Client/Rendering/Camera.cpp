#include "Camera.hpp"
#include "Common/GameObject.hpp"

Camera::Camera()
{

}

matrix4 Camera::getProjectionMatrix(int screenWidth, int screenHeight)
{
	float aspect_ratio = ((float)screenWidth) / ((float)screenHeight);

	float fovy = 2 * atan(tan(this->frame_of_view / 2) / aspect_ratio);

	//matrix4 projection_matrix = glm::tweakedInfinitePerspective(fovy, aspect_ratio, this->z_near);

	float f = 1.0f / tan(glm::radians(this->frame_of_view) / 2.0f);
	return glm::mat4(
		f / aspect_ratio, 0.0f, 0.0f, 0.0f,
		0.0f, f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, this->z_near, 0.0f);

	//return projection_matrix;
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

matrix4 Camera::getOriginViewMatrix()
{
	return this->parent->getGlobalTransform().getOriginViewMatrix();
}

void Camera::setProjection(float frame_of_view, float z_near, float z_far)
{
	this->frame_of_view = frame_of_view;
	this->z_near = z_near;
	this->z_far = z_far;
}

vector3D Camera::getPosition()
{
	return this->parent->getGlobalTransform().getPosition();
}
