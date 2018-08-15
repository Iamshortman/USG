#include "Client/GameState/GameState_Singleplayer.hpp"
#include "Client/Client.hpp"

#include "Common/Entity/Entity.hpp"
#include "Common/Entity/EntityManager.hpp"

#include "Common/World/WorldManager.hpp"

#include "Client/Component/DebugCamera.hpp"
#include "Client/Rendering/Camera.hpp"
#include "Common/Rendering/Model.hpp"
#include "Common/Rendering/Lights.hpp"

#include "Common/Component/ComponentShipFlight.hpp"

#include "Common/Component/ProjectileLauncher.hpp"
#include "Common/Component/EntityHealth.hpp"

#include "Common/Component/CharacterController.hpp"

#include "Client/Input/InputManager.hpp"

#include "Common/Entity/EntityConstructor.hpp"

NodeEntity* build_big_ship()
{
	NodeEntity* node_entity = EntityManager::getInstance()->createNodeEntity();
	node_entity->addRigidBody();

	Node* hull = new Node();
	node_entity->addChild(hull);
	hull->addNodeComponent<Model>("res/Cube_Ship/Hull.obj", "res/Cube_Ship/Hull.png", "res/shaders/Textured", "");
	hull->addNodeComponent<CollisionShape>(CollisionShapeType::ConvexMesh, "res/Cube_Ship/Hull.obj");

	Node* bridge = new Node();
	node_entity->addChild(bridge);
	bridge->addNodeComponent<Model>("res/Cube_Ship/Bridge.obj", "res/Cube_Ship/Bridge.png", "res/shaders/Textured", "");
	bridge->addNodeComponent<CollisionShape>(CollisionShapeType::ConvexMesh, "res/Cube_Ship/Bridge.obj");

	node_entity->getRigidBody()->setMass(8000000.0);
	node_entity->getRigidBody()->setInertiaTensor(vector3D(1000000000.0));
	node_entity->getRigidBody()->getRigidBody()->forceActivationState(DISABLE_DEACTIVATION);
	//node_entity->addComponent<EntityHealth>(1000.0);

	node_entity->setSubWorld(WorldManager::getInstance()->createWorld());
	node_entity->getSubWorld()->setGravity(vector3D(0.0, -9.8, 0.0));

	Entity* interior = EntityManager::getInstance()->createEntity();
	interior->addRigidBody()->setMass(0.0);
	interior->addToWorld(node_entity->getSubWorld());
	interior->addNodeComponent<Model>("res/Cube_Ship/Interior.obj", "res/Cube_Ship/Interior.png", "res/shaders/Textured", "");
	interior->addNodeComponent<CollisionShape>(CollisionShapeType::ConcaveMesh, "res/Cube_Ship/Interior.obj");

	Entity* interior_bridge = EntityManager::getInstance()->createEntity();
	interior_bridge->addRigidBody()->setMass(0.0);
	interior_bridge->addToWorld(node_entity->getSubWorld());
	interior_bridge->addNodeComponent<Model>("res/Cube_Ship/Interior_Bridge.obj", "res/Cube_Ship/Interior_Bridge.png", "res/shaders/Textured", "");
	interior_bridge->addNodeComponent<CollisionShape>(CollisionShapeType::ConcaveMesh, "res/Cube_Ship/Interior_Bridge.obj");

	return node_entity;
}


NodeEntity* build_cobra()
{
	NodeEntity* node_entity = EntityManager::getInstance()->createNodeEntity();
	node_entity->addRigidBody();

	Node* hull = new Node();
	hull->setLocalTransform(Transform());
	node_entity->addChild(hull);
	hull->addNodeComponent<Model>("res/models/Cobra/Hull.obj", "res/textures/1K_Grid.png", "res/shaders/Textured", "");
	hull->addNodeComponent<CollisionShape>(CollisionShapeType::ConvexMesh, "res/models/Cobra/Col_Hull.obj");

	Node* nose = new Node();
	nose->setLocalTransform(Transform());
	node_entity->addChild(nose);
	nose->addNodeComponent<CollisionShape>(CollisionShapeType::ConvexMesh, "res/models/Cobra/Col_Hull_Nose.obj");

	Node* right_engine = new Node();
	node_entity->addChild(right_engine);
	right_engine->setLocalTransform(Transform(vector3D(-1.7, 0.0, -4.25)));
	right_engine->addNodeComponent<Model>("res/models/Cobra/Engine_R.obj", "res/textures/1K_Grid.png", "res/shaders/Textured", "");
	right_engine->addNodeComponent<CollisionShape>(CollisionShapeType::ConvexMesh, "res/models/Cobra/Col_Engine.obj");
	{
		Node* right_wing = new Node();
		right_engine->addChild(right_wing);
		right_wing->setLocalTransform(Transform(vector3D(-2.5, 0.0, 0.7)));
		right_wing->addNodeComponent<Model>("res/models/Cobra/Wing_R.obj", "res/textures/1K_Grid.png", "res/shaders/Textured", "");
		right_wing->addNodeComponent<CollisionShape>(CollisionShapeType::ConvexMesh, "res/models/Cobra/Col_Wing_R.obj");
	}

	Node* left_engine = new Node();
	node_entity->addChild(left_engine);
	left_engine->setLocalTransform(Transform(vector3D(1.7, 0.0, -4.25)));
	left_engine->addNodeComponent<Model>("res/models/Cobra/Engine_L.obj", "res/textures/1K_Grid.png", "res/shaders/Textured", "");
	left_engine->addNodeComponent<CollisionShape>(CollisionShapeType::ConvexMesh, "res/models/Cobra/Col_Engine.obj");
	{
		Node* left_wing = new Node();
		left_engine->addChild(left_wing);
		left_wing->setLocalTransform(Transform(vector3D(2.5, 0.0, 0.7)));
		left_wing->addNodeComponent<Model>("res/models/Cobra/Wing_L.obj", "res/textures/1K_Grid.png", "res/shaders/Textured", "");
		left_wing->addNodeComponent<CollisionShape>(CollisionShapeType::ConvexMesh, "res/models/Cobra/Col_Wing_L.obj");
	}

	Node* cockpit = new Node();
	node_entity->addChild(cockpit);
	cockpit->addNodeComponent<Model>("res/models/Cobra/Cockpit.obj", "res/textures/1K_Grid.png", "res/shaders/Textured", "");

	Node* canopy = new Node();
	node_entity->addChild(canopy);
	canopy->setLocalTransform(Transform(vector3D(0.0, 1.3, -2.6)));
	canopy->addNodeComponent<Model>("res/models/Cobra/Canopy_Outside.obj", "res/textures/1K_Grid.png", "res/shaders/Textured", "");
	canopy->addNodeComponent<CollisionShape>(CollisionShapeType::ConvexMesh, "res/models/Cobra/Canopy_Outside.obj");

	Node* canopy_inside = new Node();
	canopy->addChild(canopy_inside);
	canopy_inside->addNodeComponent<Model>("res/models/Cobra/Canopy_Inside.obj", "res/textures/1K_Grid.png", "res/shaders/Textured", "");
	
	Node* gun = new Node();
	node_entity->addChild(gun);
	gun->setLocalTransform(Transform(vector3D(0.0, -0.75, 5.0)));
	gun->addNodeComponent<ProjectileLauncher>();

	node_entity->getRigidBody()->setMass(4000.0);
	node_entity->getRigidBody()->setInertiaTensor(vector3D(0.0));
	node_entity->getRigidBody()->getRigidBody()->forceActivationState(DISABLE_DEACTIVATION);
	return node_entity;
}

GameState_Singleplayer::GameState_Singleplayer()
{
	this->world = WorldManager::getInstance()->createWorld();

	NodeEntity* big_ship = build_big_ship();
	big_ship->addToWorld(this->world);
	big_ship->setLocalTransform(Transform(vector3D(0.0, 0.0, 50.0), quaternionD(0.793353, 0.0, -0.608761, 0.0)));
	big_ship->addComponent<ShipFlightController>();

	if (true)
	{
		this->character = EntityManager::getInstance()->createNodeEntity();
		this->character->addToWorld(big_ship->getSubWorld());
		this->character->addRigidBody()->setInertiaTensor(vector3D(0.0));
		this->character->getRigidBody()->getRigidBody()->forceActivationState(DISABLE_DEACTIVATION);
		this->character->addComponent<CharacterController>();
		this->character->setLocalTransform(Transform(vector3D(0.0, 0.0, 10.0)));
		Node* body = new Node();
		this->character->addChild(body);
		body->addNodeComponent<Model>("res/models/capsule.obj", "res/textures/Purple.png", "res/shaders/Textured", "");
		body->addNodeComponent<CollisionShape>(CollisionShapeType::Capsule, vector2D(0.25, 1.0));
		Node* head = new Node();
		this->character->addChild(head);
		this->character->getComponent<CharacterController>()->setHead(head);
		head->setLocalTransform(vector3D(0.0, 0.5, 0.0));
		this->character->active_camera = head->addNodeComponent<Camera>();
	}

	this->ship = (NodeEntity*)EntityConstructor::buildEntityFromJson("res/json/Ship.json");//build_cobra();
	this->ship->addComponent<ShipFlightController>();
	this->ship->addToWorld(this->world);
	this->ship->setLocalTransform(Transform(vector3D(0.0, 10.0, 0.0)));

	Node* head = new Node();
	this->ship->addChild(head);
	this->ship->active_camera = head->addNodeComponent<Camera>();
	head->setLocalTransform(vector3D(0.0, 1.1, -2.0));

	Entity* json_entity = EntityConstructor::buildEntityFromJson("res/json/Ship.json");
	json_entity->setLocalTransform(Transform(vector3D(0.0, 0.0, 20.0)));
	json_entity->addToWorld(big_ship->getSubWorld());
	json_entity->addComponent<ShipFlightController>();


	/*Entity* camera_entity = EntityManager::getInstance()->createEntity();
	this->camera = camera_entity->addNodeComponent<Camera>();
	camera_entity->addComponent<DebugCamera>(5.0, 0.25);
	camera_entity->addToWorld(this->world);*/

	this->player_controller.setPlayerEntity(this->character);
}

GameState_Singleplayer::~GameState_Singleplayer()
{
	WorldManager::getInstance()->destroyWorld(this->world->worldId);
}

void GameState_Singleplayer::update(Client* client, double delta_time)
{
	this->player_controller.applyUserInput(delta_time);

	WorldManager::getInstance()->update(delta_time);
	EntityManager::getInstance()->update();

	client->renderingManager->Render(this->player_controller.getWorld(), this->player_controller.getCamera());
}