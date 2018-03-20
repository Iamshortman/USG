#ifndef RENDERINGMANAGER_HPP
#define RENDERINGMANAGER_HPP

#include "Client/Rendering/Camera.hpp"
#include "Client/Rendering/Window.hpp"
#include "Client/Rendering/ShaderProgram.hpp"
#include "Client/Resource/TexturePool.hpp"
#include "Common/Resource/Mesh.hpp"

#include "Common/GLM_Include.hpp"
#include "Common/World/World.hpp"

#include "Common/Rendering/Lights.hpp"
#include "Common/Entity/ComponentModel.hpp"
#include "Common/Entity/ComponentLight.hpp"

//prototype
class ComponentLight;

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
	void RenderModelLight(ComponentLight* light, ComponentModel* model, Transform globalPos, Camera* camera, World* world);
	void RenderModelShadow(ComponentModel* model, Transform globalPos, Camera* camera, World* world);

	void RenderMesh(Mesh* mesh, ShaderProgram* program, Transform globalPos, Camera* camera, World* world);

private:
	Window* window = nullptr;

	bool use_lighting = true;

	GLuint shadow_map_fbo;
	GLuint shadow_map;
	uint shadow_map_size = 2048;

	double directional_light_camera_offset = 100.0;

	matrix4 lightSpaceMatrix;
	vector3D lightCameraPosition;
};

#endif //RENDERINGMANAGER_HPP
