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
	node_entity->addRigidBody();

	Node* cube1 = new Node();
	cube1->setLocalTransform(Transform());
	node_entity->addChild(cube1);
	cube1->addComponent<Model>("res/models/cube.obj", "res/textures/Green.png", "res/shaders/Textured", "", "");
	cube1->addComponent<CollisionShape>()->setBox(vector3D(0.5));

	Node* cube2 = new Node();
	cube2->setLocalTransform(Transform(vector3D(0.7, 0.6, -2.1), quaternionD(0.558268, 0.095265, -0.814123, -0.128315)));
	node_entity->addChild(cube2);
	cube2->addComponent<Model>("res/models/cube.obj", "res/textures/Purple.png", "res/shaders/Textured", "", "");
	cube2->addComponent<CollisionShape>()->setBox(vector3D(0.5));

	Node* cube3 = new Node();
	cube3->setLocalTransform(Transform(vector3D(1.41113, -0.587204, 1.45522), quaternionD(0.87165, -0.309547, -0.368961, -0.090961)));
	node_entity->addChild(cube3);
	cube3->addComponent<Model>("res/models/cube.obj", "res/textures/Blue.png", "res/shaders/Textured", "", "");
	cube3->addComponent<CollisionShape>()->setBox(vector3D(0.5));

	Node* cube4 = new Node();
	cube4->setLocalTransform(Transform(vector3D(-1.51604, 1.1474, 1.43531), quaternionD(0.856668, 0.247514, 0.32453, 0.315495)));
	node_entity->addChild(cube4);
	cube4->addComponent<Model>("res/models/cube.obj", "res/textures/Red.png", "res/shaders/Textured", "", "");
	cube4->addComponent<CollisionShape>()->setBox(vector3D(0.5));

	node_entity->getRigidBody()->setMass(200.0);
	node_entity->getRigidBody()->getRigidBody()->forceActivationState(DISABLE_DEACTIVATION);

	return node_entity;
}


GameState_Singleplayer::GameState_Singleplayer()
{
	this->world = WorldManager::getInstance()->createWorld();

	this->camera = EntityManager::getInstance()->createEntity();
	this->camera->addToWorld(this->world);
	this->camera->addComponent<Camera>();
	this->camera->addComponent<DebugCamera>(5.0, 0.5);
	this->camera->setLocalTransform(Transform(vector3D(0.0, 2.0, -10.0)));

	this->ship = EntityManager::getInstance()->createEntity();
	this->ship->addToWorld(this->world);
	this->ship->addComponent<Model>("res/models/cube.obj", "res/textures/Red.png", "res/shaders/Textured", "", "");
	this->ship->addRigidBody();
	this->ship->addComponent<CollisionShape>()->setBox(vector3D(0.5));

	Entity* entity_node = build_node();
	entity_node->addToWorld(this->world);
	entity_node->setLocalTransform(Transform(vector3D(-2.0, 0.0, 1.0)));
}

GameState_Singleplayer::~GameState_Singleplayer()
{
	WorldManager::getInstance()->destroyWorld(this->world->worldId);
}

void GameState_Singleplayer::update(Client* client, double delta_time)
{
	this->world->update(delta_time);

	client->renderingManager->Render(this->camera->getWorld(), this->camera->getComponent<Camera>());
}