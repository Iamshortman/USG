#include "RenderingManager.hpp"

#include "Common/World/WorldSolarSystem.hpp"

#include "Client/Resource/MeshPool.hpp"
#include "Client/Resource/ShaderPool.hpp"
#include "Client/Resource/TexturePool.hpp"
#include "Client/Rendering/LightManager.hpp"
#include "Client/Rendering/LightShaderUtil.hpp"

#include <stack>
#include "Common/Entity/EntityNode.hpp"

RenderingManager::RenderingManager()
{
	glGenFramebuffers(1, &shadow_map_fbo);

	glGenTextures(1, &shadow_map);
	glBindTexture(GL_TEXTURE_2D, shadow_map);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadow_map_size, shadow_map_size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, shadow_map_fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT , GL_TEXTURE_2D, shadow_map, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	ShaderPool::instance->loadShader("Textured", "res/shaders/Textured.vs", "res/shaders/Textured.fs", { { 0, "in_Position" },{ 1, "in_Normal" },{ 2, "in_TexCoord" } });
	ShaderPool::instance->loadShader("Textured_Lighting", "res/shaders/Textured.vs", "res/shaders/Textured_Lighting.fs", { { 0, "in_Position" },{ 1, "in_Normal" },{ 2, "in_TexCoord" } });
	ShaderPool::instance->loadShader("Textured_Shadow", "res/shaders/Textured_Shadow.vs", "res/shaders/Textured_Shadow.fs", { { 0, "in_Position" } });
	
	MeshPool::instance->loadMesh("res/models/SmallCube.obj");
	MeshPool::instance->loadMesh("res/models/CubeShip1.obj");
	MeshPool::instance->loadMesh("res/models/Player_Capsule.obj");
	MeshPool::instance->loadMesh("res/models/Cobra/Hull.obj");
	MeshPool::instance->loadMesh("res/models/Cobra/Engine.obj");
	MeshPool::instance->loadMesh("res/models/Cobra/Wing.obj");
	MeshPool::instance->loadMesh("res/models/Cobra/Canopy_Outside.obj");
	MeshPool::instance->loadMesh("res/models/Cobra/Cockpit.obj");
	MeshPool::instance->loadMesh("res/models/plane.obj");

	TexturePool::instance->loadTexture("res/textures/1K_Grid.png");
}

RenderingManager::~RenderingManager()
{

}

void RenderingManager::setWindow(Window* win)
{
	this->window = win;
}

Window* RenderingManager::getWindow()
{
	return this->window;
}

void RenderingManager::Render(World* baseWorld, Camera* cam)
{
	World* world = baseWorld;

	while (world->hasParentWorld())
	{
		world = world->getParentWorld();
	}

	this->RenderWorld(world, cam);
}

void RenderingManager::RenderWorld(World* world, Camera* camera)
{
	if (world == nullptr)
	{
		return;
	}

	DirectionalLight* light = LightManager::instance->getLightsForWorld(world->worldId)->directionalLights[0];

	//Now render world
	auto entities = world->getEntitiesInWorld();

	//Shadow Pass
	glViewport(0, 0, this->shadow_map_size, this->shadow_map_size);
	glBindFramebuffer(GL_FRAMEBUFFER, this->shadow_map_fbo);
	glClear(GL_DEPTH_BUFFER_BIT);

	//Directional placed off original camera + light direction;
	vector3D new_camera_pos = camera->getPosition() + ((vector3D)light->getDirection() * 20.0);
	Camera light_camera = Camera(new_camera_pos, light->getDirection() * -1.0f, vector3F(0.0f, 0.0, 1.0f));
	light_camera.setProjection(0.0f, 0.1f, 100.0f);
	lightSpaceMatrix = light_camera.getOrthographicMatrix(50.0f, 50.0f) * light_camera.getOriginViewMatrix();
	lightCameraPosition = light_camera.getPosition();


	for (auto it = entities->begin(); it != entities->end(); it++)
	{
		Entity* entity = *it;
		if (entity != nullptr)
		{
			switch (entity->getEntityType())
			{
			case ENTITYTYPE::ENTITY_NODE:
				EntityNode * node = (EntityNode*)entity;
				Transform renderTransform = node->getRenderTransform();
				for (ComponentModel* model : node->models)
				{
					if (model->castShadows())
					{
						this->RenderModelShadow(model, renderTransform, &light_camera, world);
					}
				}
			}
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	this->window->resetGlViewport();

	for (auto it = entities->begin(); it != entities->end(); it++)
	{
		Entity* entity = *it;

		if (entity != nullptr)
		{
			switch (entity->getEntityType())
			{
			case ENTITYTYPE::ENTITY_NODE:
				EntityNode* node = (EntityNode*) entity;
				Transform renderTransform = node->getRenderTransform();
				for (ComponentModel* model : node->models)
				{
					this->RenderModel(model, renderTransform, camera, world);
				}
			}
		}
	}

	if (this->use_lighting)
	{

	}

	auto subWorlds = world->getSubWorlds();
	for (auto it = subWorlds->begin(); it != subWorlds->end(); it++)
	{
		World* subWorld = *it;
		this->RenderWorld(subWorld, camera);
	}

}

void RenderingManager::RenderModel(ComponentModel* model, Transform globalPos, Camera * camera, World * world)
{
	Transform renderTransform = model->getParentNode()->getTransform().transformBy(globalPos);
	matrix4 modelMatrix = renderTransform.getModleMatrix(camera->getPosition());
	matrix4 mvp = camera->getProjectionMatrix(this->window) * camera->getOriginViewMatrix() * modelMatrix;

	vector3F ambientLight = world->ambientLight;
	Mesh* mesh = MeshPool::instance->getMesh(model->getMesh());
	glBindTexture(GL_TEXTURE_2D, TexturePool::instance->getTexture(model->getTexture()));


	//Ambient pass
	ShaderProgram* ambient_shader = ShaderPool::instance->getShader(model->getAmbientShader());

	ambient_shader->setActiveProgram();
	ambient_shader->setUniform("MVP", mvp);
	ambient_shader->setUniform("modelMatrix", modelMatrix);
	ambient_shader->setUniform("normalMatrix", renderTransform.getNormalMatrix());
	ambient_shader->setUniform("ambientLight", ambientLight);

	mesh->draw(ambient_shader);

	ambient_shader->deactivateProgram();
}

void RenderingManager::RenderModelLight(ComponentLight* light, ComponentModel* model, Transform globalPos, Camera* camera, World* world)
{
	Transform renderTransform = model->getParentNode()->getTransform().transformBy(globalPos);
	matrix4 modelMatrix = renderTransform.getModleMatrix(camera->getPosition());
	matrix4 mvp = camera->getProjectionMatrix(this->window) * camera->getOriginViewMatrix() * modelMatrix;

	vector3F ambientLight = world->ambientLight;

	Mesh* mesh = MeshPool::instance->getMesh(model->getMesh());
	glBindTexture(GL_TEXTURE_2D, TexturePool::instance->getTexture(model->getTexture()));
	ShaderProgram* lighting_shader = ShaderPool::instance->getShader(model->getLightingShader());

	lighting_shader->setActiveProgram();
	lighting_shader->setUniform("MVP", mvp);
	lighting_shader->setUniform("modelMatrix", modelMatrix);
	lighting_shader->setUniform("normalMatrix", renderTransform.getNormalMatrix());
	lighting_shader->setUniform("ambientLight", ambientLight);

	//setDirectionalLight("directinal_lights[0]", lighting_shader, set->directionalLights[0], Transform());
	//lighting_shader->setUniform("directinal_count", 1);
	//lighting_shader->setUniform("lightSpaceMatrix", this->lightSpaceMatrix);
	//lighting_shader->setUniform("lightModelMatrix", renderTransform.getModleMatrix(this->lightCameraPosition));

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->shadow_map);
	lighting_shader->setUniform("shadow_map", 1);

	mesh->draw(lighting_shader);

	lighting_shader->deactivateProgram();

}

void RenderingManager::RenderModelShadow(ComponentModel* model, Transform globalPos, Camera* camera, World* world)
{
	Transform renderTransform = model->getParentNode()->getTransform().transformBy(globalPos);

	matrix4 modelMatrix = renderTransform.getModleMatrix(camera->getPosition());
	matrix4 mvp = lightSpaceMatrix * modelMatrix;

	Mesh* mesh = MeshPool::instance->getMesh(model->getMesh());

	//Ambient pass
	ShaderProgram* shadow_shader = ShaderPool::instance->getShader(model->getShadowShader());

	shadow_shader->setActiveProgram();
	shadow_shader->setUniform("MVP", mvp);

	mesh->draw(shadow_shader);

	shadow_shader->deactivateProgram();
}

//Dont use, to be deprecated
void RenderingManager::RenderMesh(Mesh* mesh, ShaderProgram* program, Transform globalPos, Camera* camera, World* world)
{
	if (mesh == nullptr || program == nullptr)
	{
		return;
	}

	vector3F ambientLight = world->ambientLight;

	matrix4 projection = camera->getProjectionMatrix(this->window);
	matrix4 view = camera->getOriginViewMatrix();
	matrix4 modelMatrix = globalPos.getModleMatrix(camera->getPosition());
	matrix4 mvp = projection * view * modelMatrix;

	program->setActiveProgram();
	program->setUniform("MVP", mvp);
	program->setUniform("modelMatrix", modelMatrix);
	program->setUniform("normalMatrix", globalPos.getNormalMatrix());
	program->setUniform("ambientLight", ambientLight);

	glBindTexture(GL_TEXTURE_2D, TexturePool::instance->getTexture("res/textures/1K_Grid.png"));

	mesh->draw(program);

	program->deactivateProgram();

}