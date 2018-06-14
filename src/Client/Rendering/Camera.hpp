#ifndef CAMERA_HPP
#define CAMERA_HPP


#include "Common/Component/Component.hpp"
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
	vector3D getPosition();

	float frame_of_view = 70.0f;

private:
	//Perspective varables
	float z_near = 0.1f;
	float z_far = 1000.0f;
};

#endif // CAMERA_HPP
