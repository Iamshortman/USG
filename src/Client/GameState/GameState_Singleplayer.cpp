#include "Client/GameState/GameState_Singleplayer.hpp"
#include "Client/Client.hpp"

#include "Common/Entity/Entity.hpp"
#include "Common/Entity/EntityManager.hpp"

#include "Common/World/WorldManager.hpp"

#include "Client/Component/DebugCamera.hpp"
#include "Client/Rendering/Camera.hpp"
#include "Common/Component/Model.hpp"

#include "Common/Component/ComponentShipFlight.hpp"

#include "Common/Component/ProjectileLauncher.hpp"
#include "Common/Component/EntityHealth.hpp"


NodeEntity* build_LongShip()
{
	NodeEntity* node_entity = EntityManager::getInstance()->createNodeEntity();
	node_entity->addRigidBody();

	Node* hull = new Node();
	hull->setLocalTransform(Transform());
	node_entity->addChild(hull);
	hull->addNodeComponent<Model>("res/models/LongShip.obj", "res/textures/1K_Grid.png", "res/shaders/Textured", "", "");
	hull->addNodeComponent<CollisionShape>()->setConvexMesh("res/models/LongShip.obj");

	node_entity->getRigidBody()->setMass(8000000.0);
	node_entity->getRigidBody()->setInertiaTensor(vector3D(1000000000.0));
	node_entity->getRigidBody()->getRigidBody()->forceActivationState(DISABLE_DEACTIVATION);

	node_entity->addComponent<EntityHealth>(1000.0);

	return node_entity;
}


NodeEntity* build_node()
{
	NodeEntity* node_entity = EntityManager::getInstance()->createNodeEntity();

	node_entity->addComponent<ComponentShipFlight>();

	node_entity->addRigidBody();

	Node* hull = new Node();
	hull->setLocalTransform(Transform());
	node_entity->addChild(hull);
	hull->addNodeComponent<Model>("res/models/Cobra/Hull.obj", "res/textures/1K_Grid.png", "res/shaders/Textured", "", "");
	hull->addNodeComponent<CollisionShape>()->setConvexMesh("res/models/Cobra/Col_Hull.obj");

	Node* nose = new Node();
	nose->setLocalTransform(Transform());
	node_entity->addChild(nose);
	nose->addNodeComponent<CollisionShape>()->setConvexMesh("res/models/Cobra/Col_Hull_Nose.obj");

	Node* right_engine = new Node();
	node_entity->addChild(right_engine);
	right_engine->setLocalTransform(Transform(vector3D(-1.7, 0.0, -4.25)));
	right_engine->addNodeComponent<Model>("res/models/Cobra/Engine_R.obj", "res/textures/1K_Grid.png", "res/shaders/Textured", "", "");
	right_engine->addNodeComponent<CollisionShape>()->setConvexMesh("res/models/Cobra/Col_Engine.obj");
	{
		Node* right_wing = new Node();
		right_wing->setLocalTransform(Transform(vector3D(-2.5, 0.0, 0.7)));
		right_wing->addNodeComponent<Model>("res/models/Cobra/Wing_R.obj", "res/textures/1K_Grid.png", "res/shaders/Textured", "", "");
		right_wing->addNodeComponent<CollisionShape>()->setConvexMesh("res/models/Cobra/Col_Wing_R.obj");
		right_engine->addChild(right_wing);
	}

	Node* left_engine = new Node();
	node_entity->addChild(left_engine);
	left_engine->setLocalTransform(Transform(vector3D(1.7, 0.0, -4.25)));
	left_engine->addNodeComponent<Model>("res/models/Cobra/Engine_L.obj", "res/textures/1K_Grid.png", "res/shaders/Textured", "", "");
	left_engine->addNodeComponent<CollisionShape>()->setConvexMesh("res/models/Cobra/Col_Engine.obj");
	{
		Node* left_wing = new Node();
		left_wing->setLocalTransform(Transform(vector3D(2.5, 0.0, 0.7)));
		left_wing->addNodeComponent<Model>("res/models/Cobra/Wing_L.obj", "res/textures/1K_Grid.png", "res/shaders/Textured", "", "");
		left_wing->addNodeComponent<CollisionShape>()->setConvexMesh("res/models/Cobra/Col_Wing_L.obj");
		left_engine->addChild(left_wing);
	}

	Node* cockpit = new Node();
	node_entity->addChild(cockpit);
	cockpit->addNodeComponent<Model>("res/models/Cobra/Cockpit.obj", "res/textures/1K_Grid.png", "res/shaders/Textured", "", "");

	Node* canopy = new Node();
	node_entity->addChild(canopy);
	canopy->setLocalTransform(Transform(vector3D(0.0, 1.3, -2.6)));
	canopy->addNodeComponent<Model>("res/models/Cobra/Canopy_Outside.obj", "res/textures/1K_Grid.png", "res/shaders/Textured", "", "");
	canopy->addNodeComponent<CollisionShape>()->setConvexMesh("res/models/Cobra/Canopy_Outside.obj");
	
	Node* gun = new Node();
	node_entity->addChild(gun);
	gun->setLocalTransform(Transform(vector3D(0.0, -0.75, 5.0)));
	gun->addNodeComponent<ProjectileLauncher>();

	node_entity->getRigidBody()->setMass(5000.0);
	//node_entity->getRigidBody()->calcInertiaTensorFromShape();
	node_entity->getRigidBody()->setInertiaTensor(vector3D(1000.0));
	node_entity->getRigidBody()->getRigidBody()->forceActivationState(DISABLE_DEACTIVATION);

	return node_entity;
}


GameState_Singleplayer::GameState_Singleplayer()
{
	this->world = WorldManager::getInstance()->createWorld();

	NodeEntity* entity_node = build_node();
	entity_node->addToWorld(this->world);
	entity_node->setLocalTransform(Transform(vector3D(-2.0, 0.0, 1.0)));
	entity_node->getRigidBody()->setLinearVelocity(vector3D(0.0, 0.0, 10.0));

	Node* camera_node = new Node();
	this->camera = camera_node->addNodeComponent<Camera>();
	camera_node->setLocalTransform(Transform(vector3D(0.0, 1.2, -2.0)));
	entity_node->addChild(camera_node);


	NodeEntity* long_ship = build_LongShip();
	long_ship->addToWorld(this->world);
	long_ship->setLocalTransform(Transform(vector3D(0.0, 0.0, 500.0), quaternionD(0.793353, 0.0, -0.608761, 0.0)));
}

GameState_Singleplayer::~GameState_Singleplayer()
{
	WorldManager::getInstance()->destroyWorld(this->world->worldId);
}

void GameState_Singleplayer::update(Client* client, double delta_time)
{
	WorldManager::getInstance()->update(delta_time);
	EntityManager::getInstance()->update();

	client->renderingManager->Render(this->camera->parent_node->getEntity()->getWorld(), this->camera);
}