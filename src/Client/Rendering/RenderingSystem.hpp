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
#include "Common/Rendering/Model.hpp"
#include "Common/Rendering/Lights.hpp"

#include <vector>

class RenderingSystem
{
public:
	RenderingSystem();
	virtual ~RenderingSystem();

	void generateShadowMaps() {};

	void renderMS(GLuint render_target, G_Buffer* ms_g_buffer, G_Buffer* g_buffer, Camera* camera);
	void render(GLuint render_target, G_Buffer* g_buffer, Camera* camera);

	void addModel(Model* model, Transform global_transform);
	void addPointLight(PointLight* point, Transform global_transform);
	void addSpotLight(SpotLight* spot, Transform global_transform);

	void setSkybox(Skybox* box);
	void setAmbientLight(vector3F ambient_light);

	void clearScene();

private:
	void RenderModel(Mesh* mesh, GLuint& texture, ShaderProgram* program, Transform& transform, Camera* camera, vector2I& screen_size);

	void generateGBuffer(G_Buffer* g_buffer, Camera* camera);
	void drawAmbient(GLuint render_target, G_Buffer* g_buffer, Camera* camera);
	void drawLights(GLuint render_target, G_Buffer* g_buffer, Camera* camera);

	//std::map<Model, std::vector<Transform>> models;
	std::vector<std::pair<Model*, Transform>> models;

	std::vector<std::pair<PointLight*, Transform>> point_lights;
	std::vector<std::pair<SpotLight*, Transform>> spot_lights;


	Skybox* skybox = nullptr;
	vector3F ambient_light = vector3F(0.1f);

	ShaderProgram* full_screen_quad_program = nullptr;
	TexturedMesh* full_screen_quad = nullptr;

	ShaderProgram* deferred_ambient = nullptr;
	ShaderProgram* deferred_light_directional = nullptr;
	ShaderProgram* deferred_light_point = nullptr;
	ShaderProgram* deferred_light_spot = nullptr;


	//Render Settings
	bool use_lighting = true;
};

#endif //RENDERING_SYSTEM_HPP
