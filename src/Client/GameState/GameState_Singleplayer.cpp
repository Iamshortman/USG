#include "Client/GameState/GameState_Singleplayer.hpp"
#include "Client/Client.hpp"

#include "Common/Entity/Entity.hpp"
#include "Common/Entity/EntityManager.hpp"

#include "Common/World/WorldManager.hpp"

#include "Client/Component/DebugCamera.hpp"
#include "Client/Rendering/Camera.hpp"
#include "Common/Component/ComponentModel.hpp"

#include "Common/Component/ComponentShipFlight.hpp"

GameState_Singleplayer::GameState_Singleplayer()
{
	this->world = WorldManager::getInstance()->createWorld();
	this->world->setGravity(vector3D(0.0, -4.9, 0.0));

	this->camera = EntityManager::getInstance()->createEntity();
	this->camera->addToWorld(this->world);
	this->camera->addComponent<Camera>();
	this->camera->addComponent<DebugCamera>(5.0, 0.5);
	this->camera->setLocalTransform(Transform(vector3D(0.0, 2.0, -10.0)));


	this->ship = EntityManager::getInstance()->createEntity();
	this->ship->addToWorld(this->world);
	this->ship->addComponent<ComponentModel>("res/models/A-Wing/a-wing-body.obj", "res/textures/A-Wing/a-wing-body.png", "res/shaders/Textured", "", "");
	this->ship->addRigidBody();
	this->ship->addComponent<CollisionShape>()->setBox(vector3D(2.0, 0.5, 3.0));


	Entity* ground = EntityManager::getInstance()->createEntity();
	ground->setLocalTransform(Transform(vector3D(0.0, -3.0, 0.0)));

	ground->addToWorld(this->world);
	ground->addComponent<ComponentModel>("res/models/A-Wing/a-wing-body.obj", "res/textures/A-Wing/a-wing-body.png", "res/shaders/Textured", "", "");
	ground->addRigidBody();
	ground->addComponent<CollisionShape>()->setBox(vector3D(5.0, 0.5, 5.0));
	ground->getRigidBody()->setMass(0.0);

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