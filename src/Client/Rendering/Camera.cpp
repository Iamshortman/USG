#include "Camera.hpp"
#include "Common/GameObject.hpp"

Camera::Camera()
{

}

matrix4 Camera::getProjectionMatrix(int screenWidth, int screenHeight)
{
	float aspectRatio = ((float)screenWidth) / ((float)screenHeight);

	float fovy = 2 * atan(tan(this->frame_of_view / 2) / aspectRatio);

	return glm::tweakedInfinitePerspective(fovy, aspectRatio, this->z_near);
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
