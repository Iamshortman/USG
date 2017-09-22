#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Common/GLM_Include.hpp"
#include "Common/Transform.hpp"
#include "Client/Rendering/Window.hpp"

class Camera
{
public:
	Camera();
	matrix4 getViewMatrix();
	matrix4 getOriginViewMatrix();
	matrix4 getProjectionMatrix(int screenWidth, int screenheight);
	matrix4 getProjectionMatrix(Window* window);
	matrix4 getModelMatrix();

    void moveCameraPos(const vector3D& dist);
    void rotateCamera(const vector3D& direction, double angle);
    void setCameraPos(vector3D& pos);
	void setCameraTransform(vector3D position, quaternionD orientation);
	void setCameraTransform(Transform& transform);

    vector3D getForward();
    vector3D getRight();
    vector3D getUp();
    vector3D getPosition();
	quaternionF getOrientation();

	void setProjection(float frameOfView, float nearClipping, float farClipping);

private:
    vector3D forward;
	vector3D up;
    vector3D position;

	//Perspective varables
	float isPerspective = true;
	float frameOfView = 45.0f;
	float nearClipping = 0.1f;
	float farClipping = 1000.0f;

};

#endif // CAMERA_HPP
