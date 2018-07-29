#ifndef RENDERING_SYSTEM_HPP
#define RENDERING_SYSTEM_HPP

#include "Client/Rendering/Camera.hpp"
#include "Client/Rendering/ShaderProgram.hpp"
#include "Client/Resource/TexturePool.hpp"
#include "Client/Resource/TexturedMesh.hpp"
#include "Client/Rendering/G_Buffer.hpp"
#include "Client/Rendering/Skybox.hpp"

#include "Common/Resource/Mesh.hpp"
#include "Common/GLM_Include.hpp"

#include "Client/Rendering/OpenGL_Include.hpp"
#include "Common/Component/Model.hpp"

#include <vector>

class RenderingSystem
{
public:
	RenderingSystem();
	virtual ~RenderingSystem();

	void generateShadowMaps() {};

	void renderMS(GLuint render_target, G_Buffer* ms_g_buffer, G_Buffer* g_buffer, Camera* camera);
	void render(GLuint render_target, G_Buffer* g_buffer, Camera* camera) {};

	void addModel(Model* model, Transform global_transform);
	void addLight(void* light, Transform global_transform);

	void setSkybox(Skybox* box);

	void clearScene();

private:
	void RenderModel(Mesh* mesh, GLuint& texture, ShaderProgram* program, Transform& transform, Camera* camera, vector2I& screen_size);

	//std::map<Model, std::vector<Transform>> models;
	std::vector<std::pair<Model*, Transform>> models;

	Skybox* skybox = nullptr;

	ShaderProgram* full_screen_quad_program = nullptr;
	TexturedMesh* full_screen_quad = nullptr;

	//Render Settings
	bool use_lighting = true;
};

#endif //RENDERING_SYSTEM_HPP
