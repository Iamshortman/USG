#include "Client/GameState/GameState_Singleplayer.hpp"
#include "Client/Client.hpp"

#include "Common/Entity/Entity.hpp"
#include "Common/Entity/EntityManager.hpp"

#include "Common/World/WorldManager.hpp"

#include "Client/Component/DebugCamera.hpp"
#include "Client/Rendering/Camera.hpp"
#include "Common/Component/Model.hpp"

#include "Common/Component/ComponentShipFlight.hpp"

NodeEntity* build_node()
{
	NodeEntity* node_entity = EntityManager::getInstance()->createNodeEntity();

	node_entity->addComponent<ComponentShipFlight>();

	node_entity->addRigidBody();

	Node* hull = new Node();
	hull->setLocalTransform(Transform());
	node_entity->addChild(hull);
	hull->addComponent<Model>("res/models/Cobra/Hull.obj", "res/textures/1K_Grid.png", "res/shaders/Textured", "", "");
	hull->addComponent<CollisionShape>()->setConvexMesh("res/models/Cobra/Col_Hull.obj");

	Node* nose = new Node();
	nose->setLocalTransform(Transform());
	node_entity->addChild(nose);
	nose->addComponent<CollisionShape>()->setConvexMesh("res/models/Cobra/Col_Hull_Nose.obj");

	Node* right_engine = new Node();
	node_entity->addChild(right_engine);
	right_engine->setLocalTransform(Transform(vector3D(-1.7, 0.0, -4.25)));
	right_engine->addComponent<Model>("res/models/Cobra/Engine_R.obj", "res/textures/1K_Grid.png", "res/shaders/Textured", "", "");
	right_engine->addComponent<CollisionShape>()->setConvexMesh("res/models/Cobra/Col_Engine.obj");
	{
		Node* right_wing = new Node();
		right_wing->setLocalTransform(Transform(vector3D(-2.5, 0.0, 0.7)));
		right_wing->addComponent<Model>("res/models/Cobra/Wing_R.obj", "res/textures/1K_Grid.png", "res/shaders/Textured", "", "");
		right_wing->addComponent<CollisionShape>()->setConvexMesh("res/models/Cobra/Col_Wing_R.obj");
		right_engine->addChild(right_wing);
	}

	Node* left_engine = new Node();
	node_entity->addChild(left_engine);
	left_engine->setLocalTransform(Transform(vector3D(1.7, 0.0, -4.25)));
	left_engine->addComponent<Model>("res/models/Cobra/Engine_L.obj", "res/textures/1K_Grid.png", "res/shaders/Textured", "", "");
	left_engine->addComponent<CollisionShape>()->setConvexMesh("res/models/Cobra/Col_Engine.obj");
	{
		Node* left_wing = new Node();
		left_wing->setLocalTransform(Transform(vector3D(2.5, 0.0, 0.7)));
		left_wing->addComponent<Model>("res/models/Cobra/Wing_L.obj", "res/textures/1K_Grid.png", "res/shaders/Textured", "", "");
		left_wing->addComponent<CollisionShape>()->setConvexMesh("res/models/Cobra/Col_Wing_L.obj");
		left_engine->addChild(left_wing);
	}

	Node* cockpit = new Node();
	node_entity->addChild(cockpit);
	cockpit->addComponent<Model>("res/models/Cobra/Cockpit.obj", "res/textures/1K_Grid.png", "res/shaders/Textured", "", "");

	Node* canopy = new Node();
	node_entity->addChild(canopy);
	canopy->setLocalTransform(Transform(vector3D(0.0, 1.3, -2.6)));
	canopy->addComponent<Model>("res/models/Cobra/Canopy_Outside.obj", "res/textures/1K_Grid.png", "res/shaders/Textured", "", "");
	canopy->addComponent<CollisionShape>()->setConvexMesh("res/models/Cobra/Canopy_Outside.obj");

	node_entity->getRigidBody()->setMass(5000.0);
	//node_entity->getRigidBody()->calcInertiaTensorFromShape();
	node_entity->getRigidBody()->setInertiaTensor(vector3D(1000.0));
	node_entity->getRigidBody()->getRigidBody()->forceActivationState(DISABLE_DEACTIVATION);

	return node_entity;
}


GameState_Singleplayer::GameState_Singleplayer()
{
	this->world = WorldManager::getInstance()->createWorld();

	/*this->camera = EntityManager::getInstance()->createEntity();
	this->camera->addToWorld(this->world);
	this->camera->addComponent<Camera>();
	this->camera->addComponent<DebugCamera>(5.0, 0.5);
	this->camera->setLocalTransform(Transform(vector3D(0.0, 2.0, -10.0)));*/

	this->cube = EntityManager::getInstance()->createEntity();
	this->cube->addToWorld(this->world);
	this->cube->addComponent<Model>("res/models/cube.obj", "res/textures/Red.png", "res/shaders/Textured", "", "");
	this->cube->addRigidBody();
	this->cube->addComponent<CollisionShape>()->setBox(vector3D(0.5));

	NodeEntity* entity_node = build_node();
	entity_node->addToWorld(this->world);
	entity_node->setLocalTransform(Transform(vector3D(-2.0, 0.0, 1.0)));
	entity_node->getRigidBody()->setLinearVelocity(vector3D(0.0, 0.0, 10.0));

	Node* camera_node = new Node();
	this->camera = camera_node->addComponent<Camera>();
	camera_node->setLocalTransform(Transform(vector3D(0.0, 1.2, -2.0)));
	entity_node->addChild(camera_node);
}

GameState_Singleplayer::~GameState_Singleplayer()
{
	WorldManager::getInstance()->destroyWorld(this->world->worldId);
}

void GameState_Singleplayer::update(Client* client, double delta_time)
{
	this->world->update(delta_time);

	client->renderingManager->Render(this->camera->getParentNode()->getParentEntity()->getWorld(), this->camera);
}