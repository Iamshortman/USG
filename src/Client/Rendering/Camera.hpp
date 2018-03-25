#ifndef CAMERA_HPP
#define CAMERA_HPP


#include "Common/Component.hpp"
#include "Common/GLM_Include.hpp"
#include "Client/Rendering/Window.hpp"

class Camera : public Component
{
public:
	Camera();

	matrix4 getProjectionMatrix(int screenWidth, int screenheight);
	matrix4 getProjectionMatrix(Window* window);
	matrix4 getOrthographicMatrix(float x_bounds, float y_bounds);

	matrix4 getOriginViewMatrix();

	void setProjection(float frame_of_view, float z_near, float z_far);

	vector3D getPosition();

private:
	//Perspective varables
	float frame_of_view = 90.0f;
	float z_near = 0.1f;
	float z_far = 1000.0f;
};

#endif // CAMERA_HPP
