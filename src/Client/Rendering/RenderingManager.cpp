#include "RenderingManager.hpp"

#include "Common/World/WorldSolarSystem.hpp"

#include "Client/Resource/MeshPool.hpp"
#include "Client/Resource/ShaderPool.hpp"
#include "Client/Resource/TexturePool.hpp"

#include <stack>
#include "Common/Entity/EntityNode.hpp"

RenderingManager::RenderingManager()
{
	ShaderPool::instance->loadShader("Textured", "res/shaders/Textured.vs", "res/shaders/Textured.fs", { { 0, "in_Position" },{ 1, "in_Normal" },{ 2, "in_TexCoord" } });
	MeshPool::instance->loadModel("SmallCube", "res/models/SmallCube.obj", true);
	MeshPool::instance->loadModel("tempShip", "res/models/CubeShip1.obj", true);
	MeshPool::instance->loadModel("player", "res/models/Player_Capsule.obj", true);

	MeshPool::instance->loadModel("tempShipOut", "res/models/ship/outside.obj", true);
	MeshPool::instance->loadModel("tempShipIn", "res/models/ship/inside.obj", true);

	TexturePool::instance->loadTexture("res/textures/1K_Grid.png");

	tempModel = new Model();
	tempModel->setShader("Textured");
	tempModel->setMesh("SmallCube");
	tempModel->addTexture("res/textures/1K_Grid.png", 0);

	tempModel1 = new Model();
	tempModel1->setShader("Textured");
	tempModel1->setMesh("tempShipOut");
	tempModel1->addTexture("res/textures/1K_Grid.png", 0);

	tempModel2 = new Model();
	tempModel2->setShader("Textured");
	tempModel2->setMesh("tempShipIn");
	tempModel2->addTexture("res/textures/1K_Grid.png", 0);

	playerModel = new Model();
	playerModel->setShader("Textured");
	playerModel->setMesh("player");
	playerModel->addTexture("res/textures/1K_Grid.png", 0);
}

RenderingManager::~RenderingManager()
{
	delete this->tempModel;
	delete this->tempModel1;
	delete this->playerModel;
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
			case ENTITYTYPE::GRIDSYSTEM:
				this->RenderMesh(this->tempModel, entity->getRenderTransform(), cam, world);
				break;
			case ENTITYTYPE::CHARACTOR:
				this->RenderMesh(this->playerModel, entity->getRenderTransform(), cam, world);
				break;
			case ENTITYTYPE::TEMPSHIP:
				this->RenderMesh(this->tempModel1, entity->getRenderTransform(), cam, world);
				break;
			case ENTITYTYPE::TEMPSHIPINSIDE:
				this->RenderMesh(this->tempModel2, entity->getRenderTransform(), cam, world);
				break;
			case ENTITYTYPE::ENTITY_NODE:
				std::stack<Node*> nodes;
				nodes.push(((EntityNode*)entity)->rootNode);
				while (!nodes.empty())
				{
					Node* node = nodes.top();
					nodes.pop();

					this->RenderMesh(MeshPool::instance->getModel("SmallCube"), ShaderPool::instance->getShader("Textured"), node->getTransform().transformBy(entity->getRenderTransform()), cam, world);

					for (Node* child : node->child_nodes)
					{
						nodes.push(child);
					}
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

void RenderingManager::RenderMesh(Model* model, Transform globalPos, Camera* cam, World* world)
{
	ShaderProgram* program = model->getShader();
	Mesh* mesh = model->getMesh();
	RenderController* controller = model->getController();
	vector3F ambientLight = world->ambientLight;

	if (mesh == nullptr || program == nullptr)
	{
		return;
	}

	matrix4 projection = cam->getProjectionMatrix(this->window);
	matrix4 view = cam->getOriginViewMatrix();
	matrix4 modelMatrix = globalPos.getModleMatrix(cam->getPosition());
	matrix4 mvp = projection * view * modelMatrix;

	program->setActiveProgram();
	program->setUniform("MVP", mvp);
	program->setUniform("modelMatrix", modelMatrix);
	program->setUniform("normalMatrix", globalPos.getNormalMatrix());
	program->setUniform("ambientLight", ambientLight);

	GLuint texture = model->getTexture(0);
	glBindTexture(GL_TEXTURE_2D, texture);

	if (controller != nullptr)
	{
		controller->setGLSLUniform(program);
	}

	/*if (model->skeleton != nullptr)
	{
		//matrix4 transform = glm::translate(matrix4(1.0f), vector3F(0.0f, 10.0f, 0.0f));
		//program->setUniform("boneTransforms[0]", transform);
		AnimatedMesh* animMesh = (AnimatedMesh*)mesh;
		std::stack<Bone*> bones;
		bones.push(model->skeleton->rootBone);

		for (int i = 0; i < 50; i++)
		{
			string name = "boneTransforms[" + std::to_string(i) + "]";
			program->setUniform(name, matrix4(1.0f));
		}

		Mesh* temp = MeshPool::instance->getModel("IsoSphere");

		while (!bones.empty())
		{
			Bone* bone = bones.top();
			bones.pop();

			if (animMesh->boneMap.find(bone->name) != animMesh->boneMap.end())
			{
				string index = std::to_string(animMesh->boneMap[bone->name]);

				string name = "boneTransforms[" + index + "]";

				program->setUniform(name, bone->getAnimatedMatrix());
			}


			matrix4 transform = bone->getAnimatedMatrix();
			vector4F temp1 = transform * vector4F(0.0f, 0.0f, 0.0f, 1.0f);
			
			program->setUniform("localOffset", transform);
			temp->draw(program);
			program->setUniform("localOffset", matrix4(1.0f));

			for (int i = 0; i < bone->children.size(); i++)
			{
				bones.push(bone->children[i]);
			}
		}
	}*/

	mesh->draw(program);

	program->deactivateProgram();

	//Switch to lighting Shading
	/*LightSet* lights = LightManager::instance->getLightsForWorld(world->worldId);
	program = model->getLightingShader();
	if (lights != nullptr && program != nullptr)
	{
		Transform worldOffset = world->getWorldOffsetMatrix();

		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glDepthMask(false);
		glDepthFunc(GL_EQUAL);

		program->setActiveProgram();

		size_t directionalCount = lights->directionalLights.size();
		size_t pointCount = lights->pointLights.size();
		size_t spotCount = lights->spotLights.size();

		while (directionalCount > 0 || pointCount > 0 || spotCount > 0)
		{
			int count = 0;

			for (count = 0; (count < 8) && (directionalCount > 0); count++)
			{
				setDirectionalLight("directinalLights[" + std::to_string(count) + "]", program, lights->directionalLights[directionalCount - 1], worldOffset);
				directionalCount--;
			}
			program->setUniform("directinalCount", count);

			for (count = 0; (count < 8) && (pointCount > 0); count++)
			{
				setPointLight("pointLights[" + std::to_string(count) + "]", program, lights->pointLights[pointCount - 1], worldOffset, cam->getPosition());
				pointCount--;
			}
			program->setUniform("pointCount", count);


			for (count = 0; (count < 8) && (spotCount > 0); count++)
			{
				setSpotLight("spotLights[" + std::to_string(count) + "]", program, lights->spotLights[spotCount - 1], worldOffset, cam->getPosition());
				spotCount--;
			}
			program->setUniform("spotCount", count);

			program->setActiveProgram();
			program->setUniform("MVP", mvp);
			program->setUniform("modelMatrix", modelMatrix);
			program->setUniform("normalMatrix", globalPos.getNormalMatrix());
			program->setUniform("ambientLight", ambientLight);

			GLuint texture = model->getTexture(0);
			glBindTexture(GL_TEXTURE_2D, texture);

			if (controller != nullptr)
			{
				controller->setGLSLUniform(program);
			}

			mesh->draw(program);
		}

		program->deactivateProgram();

		glDepthFunc(GL_LESS);
		glDepthMask(true);
		glDisable(GL_BLEND);
	}*/

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