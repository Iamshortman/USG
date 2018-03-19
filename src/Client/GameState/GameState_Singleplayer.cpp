#include "Client/GameState/GameState.hpp"
#include "Client/Client.hpp"
#include "Common/World/WorldManager.hpp"
#include "Common/Entity/EntityManager.hpp"

#include "Client/Entity/EntityPlayerClient.hpp"
#include "Common/Entity/EntityNode.hpp"

#include "Common/Entity/ComponentModel.hpp"
#include "Common/Entity/ComponentCollisionShape.hpp"
#include "Common/Physics/CollisionShapes/ConvexMeshShape.hpp"
#include "Common/Entity/ComponentRotate.hpp"

#include "Client/Rendering/LightManager.hpp"

GameState_Singleplayer::GameState_Singleplayer()
{
	this->mainWorld = WorldManager::instance->createWorld(WORLDTYPE::SOLAR);

	this->mainWorld->ambientLight = vector3F(0.05f);

	EntityPlayerClient* player = (EntityPlayerClient*)EntityManager::instance->createEntity(ENTITYTYPE::PLAYER_THIS);
	this->playerInterface.bindCharacter(player);

	player->setTransform(Transform(vector3D(0.0, 0.0, -10.0)));
	player->addToWorld(this->mainWorld);

	if (true)
	{
		EntityNode* cobra = (EntityNode*)EntityManager::instance->createEntity(ENTITYTYPE::ENTITY_NODE);
		cobra->setTransform(Transform(vector3D(0.0, 0.0, 0.0)));
		cobra->addToWorld(this->mainWorld);

		Node* rootNode = cobra->rootNode;
		rootNode->addComponent(new ComponentModel("res/models/Cobra/Hull.obj", "res/textures/1K_Grid.png", "Textured", "Textured_Lighting", "Textured_Shadow"));
		rootNode->addComponent(new ComponentCollisionShape(new ConvexMeshShape("res/models/Cobra/Hull.obj"), 10000.0));

		Node* right_engine = new Node();
		right_engine->setLocalTransform(Transform(vector3D(-1.7, 0.0, -4.25)));
		right_engine->addComponent(new ComponentModel("res/models/Cobra/Engine.obj", "res/textures/1K_Grid.png", "Textured", "Textured_Lighting", "Textured_Shadow"));
		right_engine->addComponent(new ComponentCollisionShape(new ConvexMeshShape("res/models/Cobra/Engine.obj"), 5000.0));
		{
			Node* right_wing = new Node();
			right_wing->setLocalTransform(Transform(vector3D(-2.5, 0.0, 0.7)));
			right_wing->addComponent(new ComponentModel("res/models/Cobra/Wing.obj", "res/textures/1K_Grid.png", "Textured", "Textured_Lighting", "Textured_Shadow"));
			right_wing->addComponent(new ComponentCollisionShape(new ConvexMeshShape("res/models/Cobra/Wing.obj"), 500.0));
			right_engine->addChild(right_wing);
		}
		rootNode->addChild(right_engine);

		Node* left_engine = new Node();
		left_engine->setLocalTransform(Transform(vector3D(1.7, 0.0, -4.25), quaternionD(0.0, 0.0, 0.0, 1.0)));
		left_engine->addComponent(new ComponentModel("res/models/Cobra/Engine.obj", "res/textures/1K_Grid.png", "Textured", "Textured_Lighting", "Textured_Shadow"));
		left_engine->addComponent(new ComponentCollisionShape(new ConvexMeshShape("res/models/Cobra/Engine.obj"), 5000.0));
		{
			Node* left_wing = new Node();
			left_wing->setLocalTransform(Transform(vector3D(-2.5, 0.0, 0.7)));
			left_wing->addComponent(new ComponentModel("res/models/Cobra/Wing.obj", "res/textures/1K_Grid.png", "Textured", "Textured_Lighting", "Textured_Shadow"));
			left_wing->addComponent(new ComponentCollisionShape(new ConvexMeshShape("res/models/Cobra/Wing.obj"), 500.0));
			left_engine->addChild(left_wing);
		}
		rootNode->addChild(left_engine);

		Node* canopy = new Node();
		canopy->setLocalTransform(Transform(vector3D(0.0, 1.3, -2.6)));
		canopy->addComponent(new ComponentModel("res/models/Cobra/Canopy_Outside.obj", "res/textures/1K_Grid.png", "Textured", "Textured_Lighting", "Textured_Shadow"));
		//canopy->addComponent(new ComponentRotate(quaternionD(), quaternionD(0.906, -0.423, 0.0, 0.0), 5.0));
		canopy->addComponent(new ComponentCollisionShape(new ConvexMeshShape("res/models/Cobra/Canopy_Outside.obj"), 100.0));
		rootNode->addChild(canopy);

		Node* cockpit = new Node();
		cockpit->addComponent(new ComponentModel("res/models/Cobra/Cockpit.obj", "res/textures/1K_Grid.png", "Textured", "Textured_Lighting", "Textured_Shadow"));
		rootNode->addChild(cockpit);
	}

	if (true)
	{
		EntityNode* plane = (EntityNode*)EntityManager::instance->createEntity(ENTITYTYPE::ENTITY_NODE);
		plane->setTransform(Transform(vector3D(0.0, -2.0, 0.0)));
		plane->addToWorld(this->mainWorld);
		plane->rootNode->addComponent(new ComponentModel("res/models/plane.obj", "res/textures/1K_Grid.png", "Textured", "Textured_Lighting", "Textured_Shadow"));
		//plane->rootNode->addComponent(new ComponentModel("res/models/plane.obj", "shadow_map", "Textured", "Textured_Lighting", "Textured_Shadow"));
		((ComponentModel*)plane->rootNode->getComponent(ComponentModel::Type))->setCastShadows(false);
	}

	DirectionalLight* light1 = new DirectionalLight(vector3F(0.0f, 1.0f, 0.0f), vector3F(1.0f), 0.8f);
	LightManager::instance->addDirectionalLight(this->mainWorld->worldId, light1);

	//PointLight* light2 = new PointLight(vector3D(0.0, 7.0, 0.0), 50.0f, vector3F(0.0f, 0.05f, 0.0f), vector3F(1.0f), 0.4f);
	//LightManager::instance->addPointLight(this->mainWorld->worldId, light2);
}

GameState_Singleplayer::~GameState_Singleplayer()
{
	if (this->mainWorld != nullptr)
	{
		delete this->mainWorld;
	}
}

void GameState_Singleplayer::update(Client* client, double deltaTime)
{
	EntityManager::instance->update();
	this->playerInterface.updatePlayerInput();
	WorldManager::instance->update(deltaTime);

	Entity* player = this->playerInterface.getBoundCharacter();
	Transform transform = this->playerInterface.getCameraTransform();
	Camera cam;
	cam.setCameraTransform(transform);
	client->window->clearBuffer();
	if (player != nullptr)
	{
		client->renderingManager->Render(player->getWorld(), &cam);
	}
	client->window->updateBuffer();
}