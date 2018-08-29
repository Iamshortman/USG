#ifndef RENDERING_ENGINE_HPP
#define RENDERING_ENGINE_HPP

#include "Client/Rendering/Camera.hpp"
#include "Client/Rendering/ShaderProgram.hpp"
#include "Client/Resource/TexturePool.hpp"
#include "Client/Resource/TexturedMesh.hpp"
#include "Client/Rendering/G_Buffer.hpp"
#include "Client/Rendering/ShadowMap.hpp"

#include "Client/Rendering/Skybox.hpp"

#include "Common/Resource/Mesh.hpp"
#include "Common/GLM_Include.hpp"

#include "Common/Transform.hpp"

#include "Client/Rendering/OpenGL_Include.hpp"
#include "Common/Rendering/Model.hpp"
#include "Common/Rendering/Lights.hpp"

#include <vector>

class RenderingEngine
{
public:
	RenderingEngine();
	virtual ~RenderingEngine();

	void setBufferSize(vector2I size);

	void renderMS(GLuint render_target, Camera* camera, Transform& camera_transform);
	void render(GLuint render_target, Camera* camera, Transform& camera_transform);

	void addModel(Model* model, Transform global_transform);
	void addDirectionalLight(DirectionalLight* directional);
	void addPointLight(PointLight* point, Transform global_transform);
	void addSpotLight(SpotLight* spot, Transform global_transform);

	void setSkybox(Skybox* box);
	void setAmbientLight(vector3F ambient_light);

	void clearScene();

private:
	void RenderModel(Mesh* mesh, GLuint& texture, ShaderProgram* program, Transform& transform, Camera* camera, Transform& camera_transform, vector2I& screen_size);
	void RenderModelShadow(Mesh* mesh, ShaderProgram* program, Transform& transform, vector3D& camera_position, matrix4& light_space_matrix);

	void generateGBuffer(G_Buffer* g_buffer, Camera* camera, Transform& camera_transform);
	void drawAmbient(GLuint render_target, G_Buffer* g_buffer, Camera* camera, Transform& camera_transform);
	void drawLights(GLuint render_target, G_Buffer* g_buffer, Camera* camera, Transform& camera_transform);
	void drawShadowMap(ShadowMap* target, vector3D& camera_position, matrix4& light_space_matrix);

	G_Buffer* g_buffer = nullptr;
	G_Buffer* ms_g_buffer = nullptr;

	ShadowMap* shadow_map = nullptr;

	std::vector<std::pair<Model*, Transform>> models;
	//std::vector<std::pair<Model*, Transform>> transparent_models;

	std::vector<DirectionalLight*> directional_lights;
	std::vector<DirectionalLight*> directional_lights_shadow;

	std::vector<std::pair<PointLight*, Transform>> point_lights;

	std::vector<std::pair<SpotLight*, Transform>> spot_lights;
	std::vector<std::pair<SpotLight*, Transform>> spot_lights_shadow;

	Skybox* skybox = nullptr;
	vector3F ambient_light = vector3F(0.5f);

	ShaderProgram* full_screen_quad_program = nullptr;
	TexturedMesh* full_screen_quad = nullptr;

	ShaderProgram* deferred_ambient = nullptr;
	ShaderProgram* deferred_light_directional = nullptr;
	ShaderProgram* deferred_light_point = nullptr;
	ShaderProgram* deferred_light_spot = nullptr;

	ShaderProgram* deferred_light_directional_shadow = nullptr;
	ShaderProgram* deferred_light_spot_shadow = nullptr;


	//Render Settings
	bool use_lighting = true;
};

#endif //RENDERING_ENGINE_HPP
