#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Common/GLM_Include.hpp"
#include "Client/Rendering/Window.hpp"

class Camera
{
public:
	Camera();

	matrix4 getProjectionMatrix(int screenWidth, int screenheight);
	matrix4 getProjectionMatrix(vector2I& screen_size);
	matrix4 getOrthographicMatrix(float x_bounds, float y_bounds);

	float frame_of_view;

private:
	//Perspective varables
	float z_near = 0.1f;
	float z_far = 1000.0f;
};

#endif // CAMERA_HPP
