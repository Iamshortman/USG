#include "Client/GameState/GameState_Singleplayer.hpp"
#include "Client/Client.hpp"

#include "Common/Entity/Entity.hpp"
#include "Common/Entity/EntityManager.hpp"

#include "Common/World/WorldManager.hpp"

#include "Client/Component/PlayerShipController.hpp"
#include "Client/Component/DebugCamera.hpp"
#include "Client/Rendering/Camera.hpp"
#include "Common/Rendering/Model.hpp"
#include "Common/Rendering/Lights.hpp"

#include "Common/Component/ComponentShipFlight.hpp"
#include "Common/Component/ShipController.hpp"

#include "Common/Component/ProjectileLauncher.hpp"
#include "Common/Component/EntityHealth.hpp"
#include "Common/Component/SeatInteract.hpp"
#include "Common/Component/Mass.hpp"

#include "Common/Component/CharacterController.hpp"

#include "Client/Input/InputManager.hpp"

#include "Common/Entity/EntityConstructor.hpp"

GameState_Singleplayer::GameState_Singleplayer()
{
	this->world = WorldManager::getInstance()->createWorld();

	ai_ship = (NodeEntity*)EntityConstructor::buildEntityFromJson("res/json/Ship.json");
	ai_ship->addToWorld(this->world);
	ai_ship->setLocalTransform(Transform(vector3D(0.0, 0.0, 0.0)));

	this->ship = (NodeEntity*)EntityConstructor::buildEntityFromJson("res/json/Ship.json");
	this->ship->addToWorld(this->world);
	this->ship->setLocalTransform(Transform(vector3D(0.0, 0.0, -50.0)));
	this->ship->addComponent<PlayerShipController>();

	Node* head = new Node();
	this->ship->addChild(head);
	this->ship->active_camera = head->addNodeComponent<Camera>();
	head->setLocalTransform(vector3D(0.0, 1.1, -2.0));

	/*character = EntityManager::getInstance()->createEntity();
	character->active_camera = character->addNodeComponent<Camera>();
	character->addComponent<DebugCamera>(5.0, 0.5);
	character->addToWorld(this->world);
	character->setLocalTransform(Transform(vector3D(5.0, 0.0, 0.0), quaternionD(0.0, 0.0, 1.0, 0.0)));*/

	this->player_controller.setPlayerEntity(ship);
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