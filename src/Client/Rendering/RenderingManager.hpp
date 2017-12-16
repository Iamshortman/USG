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

	void RenderWorld(World* world, Camera* cam);
	void Render(World* baseWorld, Camera* cam);

	void RenderMesh(Model* model, Transform globalPos, Camera* cam, World* world);
	void RenderMesh(Mesh* mesh, ShaderProgram* program, Transform globalPos, Camera* cam, World* world);


private:
	Model* tempModel = nullptr;
	Model* tempModel1 = nullptr;
	Model* tempModel2 = nullptr;

	Model* playerModel = nullptr;

	Window* window = nullptr;

	bool useLighting = false;
};

#endif //RENDERINGMANAGER_HPP
