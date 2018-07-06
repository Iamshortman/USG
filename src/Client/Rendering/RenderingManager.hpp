#ifndef RENDERINGMANAGER_HPP
#define RENDERINGMANAGER_HPP

#include "Client/Rendering/Camera.hpp"
#include "Client/Rendering/Window.hpp"
#include "Client/Rendering/ShaderProgram.hpp"
#include "Client/Resource/TexturePool.hpp"
#include "Client/Resource/TexturedMesh.hpp"
#include "Client/Rendering/G_Buffer.hpp"
#include "Client/Rendering/Skybox.hpp"

#include "Common/Resource/Mesh.hpp"
#include "Common/GLM_Include.hpp"
#include "Common/Rendering/Model.hpp"
#include "Common/Rendering/Lights.hpp"
#include "Common/Transform.hpp"
#include "Common/WorldSystem.hpp"

#include <map>
#include <queue>

class RenderingManager
{
public:
	RenderingManager(Window* window);
	virtual ~RenderingManager();

	void renderScene();

	void setCamera(Camera camera, Transform global_transform);

	void addModel(Model* model, Transform global_transform, WorldId world_id);
	void addLight(BaseLight* light, Transform global_transform, WorldId world_id);

private:
	//Storage for models and Lights
	std::map<WorldId, std::queue<std::pair<Model*, Transform>>> models;

	//Settings
	bool use_lighting = true;

	// Internal Functions
	void RenderModel(Model* model, Camera* camera, Transform& model_transform, Transform& camera_tranform);

	//Variables
	Camera camera;
	Transform camera_transform;

	Skybox* skybox = nullptr;
	Window* window = nullptr;

	string current_texture;

	//Deferred Shading
	G_Buffer* g_buffer = nullptr;
	G_Buffer* ms_g_buffer = nullptr;

	ShaderProgram* full_screen_quad_program = nullptr;
	TexturedMesh* full_screen_quad = nullptr;
};

#endif //RENDERINGMANAGER_HPP
