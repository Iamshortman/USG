#include "RenderingManager.hpp"

#include "Common/World/WorldSolarSystem.hpp"

#include "Client/Resource/MeshPool.hpp"
#include "Client/Resource/ShaderPool.hpp"
#include "Client/Resource/TexturePool.hpp"

#include <stack>
#include "Common/Entity/EntityNode.hpp"
#include "Common/Entity/ComponentModel.hpp"

RenderingManager::RenderingManager()
{
	ShaderPool::instance->loadShader("Textured", "res/shaders/Textured.vs", "res/shaders/Textured.fs", { { 0, "in_Position" },{ 1, "in_Normal" },{ 2, "in_TexCoord" } });

	MeshPool::instance->loadMesh("res/models/SmallCube.obj");
	MeshPool::instance->loadMesh("res/models/CubeShip1.obj");
	MeshPool::instance->loadMesh("res/models/Player_Capsule.obj");

	//MeshPool::instance->loadModel("tempShipOut", "res/models/ship/outside.obj");
	//MeshPool::instance->loadModel("tempShipIn", "res/models/ship/inside.obj");

	TexturePool::instance->loadTexture("res/textures/1K_Grid.png");


	MeshPool::instance->loadMesh("res/models/Cobra/Hull.obj");
	MeshPool::instance->loadMesh("res/models/Cobra/Engine.obj");
	MeshPool::instance->loadMesh("res/models/Cobra/Wing.obj");
	MeshPool::instance->loadMesh("res/models/Cobra/Canopy_Outside.obj");
	MeshPool::instance->loadMesh("res/models/Cobra/Cockpit.obj");
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

void RenderingManager::RenderWorld(World* world, Camera* cam)
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
				for (ComponentModel* model : node->models)
				{
					this->RenderMesh(MeshPool::instance->getMesh(model->getMesh()), ShaderPool::instance->getShader(model->getShader()), model->getParentNode()->getTransform().transformBy(entity->getRenderTransform()), cam, world);
				}
			}
		}

	}

	//TODO Rendering Sub Worlds
	auto subWorlds = world->getSubWorlds();

	for (auto it = subWorlds->begin(); it != subWorlds->end(); it++)
	{
		World* subWorld = *it;
		this->RenderWorld(subWorld, cam);
	}

}

void RenderingManager::RenderMesh(Mesh* mesh, ShaderProgram* program, Transform globalPos, Camera* cam, World* world)
{
	if (mesh == nullptr || program == nullptr)
	{
		return;
	}

	vector3F ambientLight = world->ambientLight;

	matrix4 projection = cam->getProjectionMatrix(this->window);
	matrix4 view = cam->getOriginViewMatrix();
	matrix4 modelMatrix = globalPos.getModleMatrix(cam->getPosition());
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