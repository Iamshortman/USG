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

#include "Client/Rendering/RenderingSystem.hpp"

class RenderingManager
{
public:
	RenderingManager(Window* window);
	virtual ~RenderingManager();

	void Render(World* world, Camera* camera);

private:
	void SubmitWorld(World* world);


	Skybox* skybox = nullptr;

	Window* window = nullptr;

	//Deferred Shading
	G_Buffer* g_buffer = nullptr;
	G_Buffer* ms_g_buffer = nullptr;

	RenderingSystem* rendering_system = nullptr;
};

#endif //RENDERINGMANAGER_HPP
