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
	ShaderPool::instance->loadShader("Textured", "res/shaders/Textured.vs", "res/shaders/Textured.fs", { { 0, "in_Position" },{ 1, "in_Normal" },{ 2, "in_TexCoord" } });
	ShaderPool::instance->loadShader("Textured_Lighting", "res/shaders/Textured.vs", "res/shaders/TexturedLighting.fs", { { 0, "in_Position" },{ 1, "in_Normal" },{ 2, "in_TexCoord" } });
	ShaderPool::instance->setUsing("Textured_Lighting");
	
	//ShaderPool::instance->loadShader("Textured_Shadow", "res/shaders/Textured.vs", "res/shaders/Textured.fs", { { 0, "in_Position" },{ 1, "in_Normal" },{ 2, "in_TexCoord" } });

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

	if (world->getWorldType() == WORLDTYPE::SOLAR)
	{
		WorldSolarSystem* solarSystem = (WorldSolarSystem*)world;

		/*for (Star* star : solarSystem->stars)
		{
			//TODO Render Stars
		}

		for (Planet* planet : solarSystem->planets)
		{
			//TODO Render Planets
		}*/
	}
	else if (world->getWorldType() == WORLDTYPE::WARP)
	{

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
			/*case ENTITYTYPE::CHARACTOR:
				this->RenderMesh(this->playerModel, entity->getRenderTransform(), cam, world);
				break;*/
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

	auto subWorlds = world->getSubWorlds();
	for (auto it = subWorlds->begin(); it != subWorlds->end(); it++)
	{
		World* subWorld = *it;
		this->RenderWorld(subWorld, camera);
	}

}

void RenderingManager::RenderModel(ComponentModel * model, Transform globalPos, Camera * camera, World * world)
{
	Transform renderTransform = model->getParentNode()->getTransform().transformBy(globalPos);
	matrix4 projection = camera->getProjectionMatrix(this->window);
	matrix4 view = camera->getOriginViewMatrix();
	matrix4 modelMatrix = renderTransform.getModleMatrix(camera->getPosition());
	matrix4 mvp = projection * view * modelMatrix;

	vector3F ambientLight = world->ambientLight;
	Mesh* mesh = MeshPool::instance->getMesh(model->getMesh());

	glBindTexture(GL_TEXTURE_2D, TexturePool::instance->getTexture(model->getTexture()));

	//Ambient pass
	ShaderProgram* ambient_shader = ShaderPool::instance->getShader(model->getShader());

	ambient_shader->setActiveProgram();
	ambient_shader->setUniform("MVP", mvp);
	ambient_shader->setUniform("modelMatrix", modelMatrix);
	ambient_shader->setUniform("normalMatrix", renderTransform.getNormalMatrix());
	ambient_shader->setUniform("ambientLight", ambientLight);

	mesh->draw(ambient_shader);

	ambient_shader->deactivateProgram();

	//Lighting pass
	if (true)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_EQUAL);

		LightSet* set = LightManager::instance->getLightsForWorld(world->worldId);

		size_t directional_count = set->directionalLights.size();
		size_t point_count = set->pointLights.size();
		size_t spot_count = set->spotLights.size();

		ShaderProgram* lighting_shader = ShaderPool::instance->getShader("Textured_Lighting");

		lighting_shader->setActiveProgram();
		lighting_shader->setUniform("MVP", mvp);
		lighting_shader->setUniform("modelMatrix", modelMatrix);
		lighting_shader->setUniform("normalMatrix", renderTransform.getNormalMatrix());
		lighting_shader->setUniform("ambientLight", ambientLight);

		while (directional_count > 0 || point_count > 0 || spot_count > 0)
		{
			int num_directional = 0;
			for (num_directional = 0; num_directional < 8 && directional_count > 0; num_directional++)
			{
				setDirectionalLight("directinal_lights[" + std::to_string(num_directional) + "]", lighting_shader, set->directionalLights[directional_count - 1], Transform());
				directional_count--;
			}
			lighting_shader->setUniform("directinal_count", num_directional);

			int num_point = 0;
			for (num_point = 0; num_point < 8 && point_count > 0; num_point++)
			{
				setPointLight("point_lights[" + std::to_string(num_point) + "]", lighting_shader, set->pointLights[point_count - 1], Transform(), camera->getPosition());
				point_count--;
			}
			lighting_shader->setUniform("point_count", num_point);


			int num_spot = 0;
			for (num_spot = 0; num_spot < 8 && spot_count > 0; num_spot++)
			{
				setSpotLight("spot_lights[" + std::to_string(num_spot) + "]", lighting_shader, set->spotLights[spot_count - 1], Transform(), camera->getPosition());
				spot_count--;
			}
			lighting_shader->setUniform("spot_count", num_spot);

			mesh->draw(lighting_shader);
		}

		lighting_shader->deactivateProgram();

		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
		glDisable(GL_BLEND);
	}
	
	glBindTexture(GL_TEXTURE_2D, 0);
}

//deprecated this render funcion
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