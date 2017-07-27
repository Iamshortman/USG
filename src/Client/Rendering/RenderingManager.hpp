#ifndef RENDERINGMANAGER_HPP
#define RENDERINGMANAGER_HPP

#include "Client/Rendering/Camera.hpp"
#include "Client/Rendering/Window.hpp"
#include "Client/Rendering/ShaderProgram.hpp"
#include "Client/Resource/TexturePool.hpp"

#include "Common/GLM_Include.hpp"
#include "Common/World/World.hpp"
#include "Client/Rendering/Model.hpp"

#include "Client/Rendering/Lights.hpp"

class RenderingManager
{

public:
	RenderingManager();
	virtual ~RenderingManager();

	void setWindow(Window* win);
	Window* getWindow();

	void Render(Camera* cam);
	void RenderWorld(World* world, Camera* cam);

	void RenderMesh(Model* model, Transform globalPos, Camera* cam, World* world);

protected:

private:
	Window* window = nullptr;

	bool useLighting = true;


};

#endif //RENDERINGMANAGER_HPP
