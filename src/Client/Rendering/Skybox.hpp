#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include "Common/Types.hpp"
#include "Client/Rendering/OpenGL_Include.hpp"
#include "Client/Rendering/ShaderProgram.hpp"
#include "Client/Rendering/Camera.hpp"
#include "Client/Resource/TexturedMesh.hpp"
#include "Common/Transform.hpp"

class Skybox
{
public:
	Skybox(string cube_path_base, string shader_name);
	~Skybox();

	void draw(Camera* camera, Transform& camera_transform, vector2I size);

private:
	ShaderProgram* shader_program = nullptr;
	TexturedMesh* cube_mesh = nullptr;
	
	GLuint cube_map = 0;
};

#endif //SKYBOX_HPP