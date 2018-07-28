#ifndef RENDERINGMANAGER_HPP
#define RENDERINGMANAGER_HPP

#include "Client/Rendering/Camera.hpp"
#include "Client/Rendering/ShaderProgram.hpp"
#include "Client/Resource/TexturePool.hpp"
#include "Client/Resource/TexturedMesh.hpp"
#include "Client/Rendering/G_Buffer.hpp"
#include "Client/Rendering/Skybox.hpp"

#include "Common/Resource/Mesh.hpp"
#include "Common/GLM_Include.hpp"

#include "Common/Component/Model.hpp"

#include <queue>

class RenderingSystem
{
public:
	RenderingSystem();
	virtual ~RenderingSystem();

	void generateShadowMaps() {};

	void renderMS(GLuint frame_buffer, G_Buffer& ms_g_buffer, G_Buffer* g_buffer, Camera* camera) {};
	void render(GLuint frame_buffer, G_Buffer* g_buffer, Camera* camera) {};

	void addModel(Model* mdoel, Transform global_transform) {};
	void addLight(void* light, Transform global_transform) {};

	void setSkybox(Skybox* box) {};

	void clearScene() {};

private:
	void RenderModel(Model* model, Camera* camera) {};

	std::queue<std::pair<Model*, Transform>> models;

	Skybox* skybox = nullptr;

	ShaderProgram* full_screen_quad_program = nullptr;
	TexturedMesh* full_screen_quad = nullptr;

	//Render Settings
	bool use_lighting = true;
};

#endif //RENDERINGMANAGER_HPP
