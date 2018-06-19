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

#include "Common/Entity/Entity.hpp"
#include "Common/Component/Model.hpp"

class RenderingManager
{
public:
	RenderingManager(Window* window);
	virtual ~RenderingManager();

	void renderScene(Entity* scene_root, Camera* camera);

	void RenderWorld(World* world, Camera* camera);
	void Render(World* baseWorld, Camera* camera);

	void RenderModel(Model* model, Camera* camera);

private:
	Skybox* skybox = nullptr;

	Window* window = nullptr;

	bool use_lighting = true;

	//Deferred Shading
	G_Buffer* g_buffer = nullptr;
	G_Buffer* ms_g_buffer = nullptr;

	ShaderProgram* full_screen_quad_program = nullptr;
	TexturedMesh* full_screen_quad = nullptr;

};

#endif //RENDERINGMANAGER_HPP
