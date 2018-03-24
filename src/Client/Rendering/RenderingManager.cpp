#include "RenderingManager.hpp"

#include "Common/World/WorldSolarSystem.hpp"

#include "Client/Resource/MeshPool.hpp"
#include "Client/Resource/ShaderPool.hpp"
#include "Client/Resource/TexturePool.hpp"

#include <stack>
#include "Common/Entity/EntityNode.hpp"

#include "Client/Rendering/LightShaderUtil.hpp"

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

	//Now render world
	auto entities = world->getEntitiesInWorld();
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
		for (ComponentLight* light : world->lightsInWorld)
		{
			if (light->getLightOn() == true)
			{
				matrix4 lightSpaceMatrix = matrix4(1.0f);
				vector3D lightCameraPosition = vector3D(1.0f);

				if (light->getCastShadows() == true)
				{
					lightSpaceMatrix = light->getLightSpaceMatrix();
					lightCameraPosition = light->getLightPostion();

					//Shadow Pass
					glViewport(0, 0, this->shadow_map_size, this->shadow_map_size);
					glBindFramebuffer(GL_FRAMEBUFFER, this->shadow_map_fbo);
					glClear(GL_DEPTH_BUFFER_BIT);

					auto entities = world->getEntitiesInWorld();
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
									this->RenderModelShadow(model, renderTransform, lightSpaceMatrix, lightCameraPosition);
								}
							}
						}
					}

					glBindFramebuffer(GL_FRAMEBUFFER, 0);
					this->window->resetGlViewport();
				}

				glEnable(GL_BLEND);
				glBlendFunc(GL_ONE, GL_ONE);
				glDepthMask(GL_FALSE);
				glDepthFunc(GL_EQUAL);

				auto entities = world->getEntitiesInWorld();
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
								this->RenderModelLight(light, model, renderTransform, camera, world, lightSpaceMatrix, lightCameraPosition);
							}
						}
					}
				}

				glDepthMask(GL_TRUE);
				glDepthFunc(GL_LESS);
				glDisable(GL_BLEND);
			}
		}
	}

	auto subWorlds = world->getSubWorlds();
	for (auto it = subWorlds->begin(); it != subWorlds->end(); it++)
	{
		World* subWorld = *it;
		this->RenderWorld(subWorld, camera);
	}

}

void RenderingManager::RenderModel(ComponentModel* model, Transform globalPos, Camera* camera, World* world)
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

void RenderingManager::RenderModelLight(ComponentLight* light, ComponentModel* model, Transform globalPos, Camera* camera, World* world, matrix4 lightSpaceMatrix, vector3D lightCameraPosition)
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

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->shadow_map);
	lighting_shader->setUniform("shadow_map", 1);
	lighting_shader->setUniform("lightSpaceMVP", lightSpaceMatrix * renderTransform.getModleMatrix(lightCameraPosition));

	LightType type = light->getLightType();
	lighting_shader->setUniform("lightType", type);
	
	if (type == LightType::Directional)
	{
		setDirectionalLight("directinal_light", lighting_shader, (DirectionalLight*) light->getLight(), light->getLightTransform());
	}
	else if (type == LightType::Point)
	{
		setPointLight("point_light", lighting_shader, (PointLight*)light->getLight(), light->getLightTransform(), camera->getPosition());
	}
	else if (type == LightType::Spot)
	{
		setSpotLight("spot_light", lighting_shader, (SpotLight*)light->getLight(), light->getLightTransform(), camera->getPosition());
	}

	mesh->draw(lighting_shader);

	lighting_shader->deactivateProgram();

}

void RenderingManager::RenderModelShadow(ComponentModel* model, Transform globalPos, matrix4 lightSpaceMatrix, vector3D lightCameraPosition)
{
	Transform renderTransform = model->getParentNode()->getTransform().transformBy(globalPos);
	matrix4 mvp = lightSpaceMatrix * renderTransform.getModleMatrix(lightCameraPosition);

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