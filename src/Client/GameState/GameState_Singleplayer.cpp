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
	hull->addNodeComponent<Model>("res/Cube_Ship/Hull.obj", "res/Cube_Ship/Hull.png", "res/shaders/Textured", "res/shaders/Shadow");
	hull->addNodeComponent<CollisionShape>(CollisionShapeType::ConvexMesh, "res/Cube_Ship/Hull.obj");

	Node* bridge = new Node();
	node_entity->addChild(bridge);
	bridge->addNodeComponent<Model>("res/Cube_Ship/Bridge.obj", "res/Cube_Ship/Bridge.png", "res/shaders/Textured", "res/shaders/Shadow");
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
	interior->addNodeComponent<Model>("res/Cube_Ship/Interior.obj", "res/Cube_Ship/Interior.png", "res/shaders/Textured", "res/shaders/Shadow");
	interior->addNodeComponent<CollisionShape>(CollisionShapeType::ConcaveMesh, "res/Cube_Ship/Interior.obj");

	Entity* interior_bridge = EntityManager::getInstance()->createEntity();
	interior_bridge->addRigidBody()->setMass(0.0);
	interior_bridge->addToWorld(node_entity->getSubWorld());
	interior_bridge->addNodeComponent<Model>("res/Cube_Ship/Interior_Bridge.obj", "res/Cube_Ship/Interior_Bridge.png", "res/shaders/Textured", "res/shaders/Shadow");
	interior_bridge->addNodeComponent<CollisionShape>(CollisionShapeType::ConcaveMesh, "res/Cube_Ship/Interior_Bridge.obj");

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
		body->addNodeComponent<Model>("res/models/capsule.obj", "res/textures/Purple.png", "res/shaders/Textured", "res/shaders/Shadow");
		body->addNodeComponent<CollisionShape>(CollisionShapeType::Capsule, vector2D(0.25, 1.0));
		Node* head = new Node();
		this->character->addChild(head);
		//this->character->getComponent<CharacterController>()->setHead(head);
		head->setLocalTransform(vector3D(0.0, 0.5, 0.0));
		//this->character->active_camera = head->addNodeComponent<Camera>();
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

	Entity* entity = EntityManager::getInstance()->createEntity();
	entity->setLocalTransform(Transform(vector3D(0.0, 0.0, 20.0), quaternionD(0.707107, 0.707107, 0.0, 0.0)));
	entity->addNodeComponent<SpotLight>(vector3F(0.0, 0.0, 1.0), 0.5f, 1000.0f, vector3F(0.0f, 0.01f, 0.0f), vector3F(1.0f), 0.4f);
	entity->addToWorld(big_ship->getSubWorld());

	Entity* camera_entity = EntityManager::getInstance()->createEntity();
	camera_entity->active_camera = camera_entity->addNodeComponent<Camera>();
	camera_entity->addComponent<DebugCamera>(5.0, 0.25);
	camera_entity->addToWorld(big_ship->getSubWorld());

	this->player_controller.setPlayerEntity(camera_entity);
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