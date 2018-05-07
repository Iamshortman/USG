#ifndef RENDERINGMANAGER_HPP
#define RENDERINGMANAGER_HPP

#include "Client/Rendering/Camera.hpp"
#include "Client/Rendering/Window.hpp"
#include "Client/Rendering/ShaderProgram.hpp"
#include "Client/Resource/TexturePool.hpp"
#include "Client/Rendering/G_Buffer.hpp"


#include "Common/Resource/Mesh.hpp"
#include "Common/GLM_Include.hpp"

#include "Common/GameObject.hpp"
#include "Common/Component/ComponentModel.hpp"

class RenderingManager
{
public:
	RenderingManager(Window* window);
	virtual ~RenderingManager();

	void renderScene(GameObject* scene_root, Camera* camera);

	void RenderModel(ComponentModel* model, Camera* camera);


private:
	Window* window = nullptr;
	G_Buffer* g_buffer = nullptr;


	bool use_lighting = true;


};

#endif //RENDERINGMANAGER_HPP
