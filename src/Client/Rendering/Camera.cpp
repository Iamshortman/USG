#include "Camera.hpp"
#include <iostream>

Camera::Camera()
{
	this->position = vector3D(0.0, 0.0, 0.0);
	this->forward = vector3F(0.0f, 0.0f, 1.0F);
	this->up = vector3F(0.0f, 1.0F, 0.0f);
}

Camera::Camera(vector3D position, vector3F forward, vector3F up)
{
	this->position = position;
	this->forward = forward;
	this->up = up;
}

matrix4 Camera::getProjectionMatrix(int screenWidth, int screenHeight)
{
	float aspectRatio = ((float)screenWidth) / ((float)screenHeight);

	//float fovY = abs((frameOfView * (float)height) / ((float)width));

	return glm::tweakedInfinitePerspective(this->frameOfView, aspectRatio, this->nearClipping);
}

matrix4 Camera::getProjectionMatrix(Window* window)
{
	int width, height;
	window->getWindowSize(width, height);
	return this->getProjectionMatrix(width, height);
}

matrix4 Camera::getOrthographicMatrix(float x_bounds, float y_bounds)
{
	return glm::ortho(-x_bounds, x_bounds, -y_bounds, y_bounds, this->nearClipping, this->farClipping);
}

matrix4 Camera::getViewMatrix()
{
	return glm::lookAt((vector3F)this->position, (vector3F)(this->position + this->forward), (vector3F)this->up);
}

matrix4 Camera::getOriginViewMatrix()
{
	vector3F forwardVec = (vector3F)this->forward;
	vector3F upVec = (vector3F)this->up;

	return glm::lookAt(vector3F(0.0f), forwardVec, upVec);
}

matrix4 Camera::getModelMatrix()
{
	matrix4 positionMatrix = matrix4();
	matrix4 rotationMatrix = matrix4();

	positionMatrix = glm::translate(matrix4(1.0F), (vector3F)this->position);
	rotationMatrix = glm::toMat4(this->getOrientation());

	return positionMatrix * rotationMatrix;
}

void Camera::setCameraPos(vector3D& pos)
{
	this->position = pos;
}

void Camera::moveCameraPos(const vector3D& dist)
{
	this->position += dist;
}

void Camera::rotateCamera(const vector3D& direction, double angle)
{
	forward = glm::normalize( glm::rotate(forward, angle, direction) );
	up = glm::normalize( glm::rotate(up, angle, direction) );

}

void Camera::setCameraTransform(vector3D pos, quaternionD orientation)
{
	this->position = pos;
	this->forward = glm::normalize(orientation * vector3D(0.0f, 0.0f, 1.0F));
	this->up = glm::normalize(orientation * vector3D(0.0f, 1.0F, 0.0f));
}

void Camera::setCameraTransform(Transform& transform)
{
	this->setCameraTransform(transform.getPosition(), transform.getOrientation());
}

vector3D Camera::getForward()
{
	return this->forward;
}

vector3D Camera::getUp()
{
	return this->up;
}

vector3D Camera::getRight()
{
	return glm::cross(this->forward, this->up);
}

vector3D Camera::getPosition()
{
	return this->position;
}

quaternionF Camera::getOrientation()
{
	vector3F right = glm::normalize(glm::cross(this->forward, this->up));

	matrix3 rotationMatrix = matrix3();

	rotationMatrix[0][0] = right.x;
	rotationMatrix[0][1] = right.y;
	rotationMatrix[0][2] = right.z;

	rotationMatrix[1][0] = (float) this->up.x;
	rotationMatrix[1][1] = (float) this->up.y;
	rotationMatrix[1][2] = (float) this->up.z;

	rotationMatrix[2][0] = (float) this->forward.x;
	rotationMatrix[2][1] = (float) this->forward.y;
	rotationMatrix[2][2] = (float) this->forward.z;

	return glm::toQuat(rotationMatrix);
}

void Camera::setProjection(float frameOfView, float nearClipping, float farClipping)
{
	this->frameOfView = frameOfView;
	this->nearClipping = nearClipping;
	this->farClipping = farClipping;
}