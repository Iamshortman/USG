#ifndef RENDERINGMANAGER_HPP
#define RENDERINGMANAGER_HPP

#include "Client/Rendering/Camera.hpp"
#include "Client/Rendering/Window.hpp"
#include "Client/Rendering/ShaderProgram.hpp"
#include "Client/Resource/TexturePool.hpp"
#include "Common/Resource/Mesh.hpp"

#include "Common/GLM_Include.hpp"
#include "Common/World/World.hpp"

#include "Client/Rendering/Lights.hpp"
#include "Common/Entity/ComponentModel.hpp"

class RenderingManager
{

public:
	RenderingManager();
	virtual ~RenderingManager();

	void setWindow(Window* win);
	Window* getWindow();

	void RenderWorld(World* world, Camera* camera);
	void Render(World* baseWorld, Camera* camera);

	void RenderModel(ComponentModel* model, Transform globalPos, Camera* camera, World* world);
	void RenderMesh(Mesh* mesh, ShaderProgram* program, Transform globalPos, Camera* camera, World* world);

private:
	Window* window = nullptr;

	bool useLighting = false;
};

#endif //RENDERINGMANAGER_HPP
