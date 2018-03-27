#include "Client/GameState/GameState.hpp"
#include "Client/Client.hpp"

#include "Common/GameObject.hpp"
#include "Common/GameObjectManager.hpp"
#include "Client/Rendering/Camera.hpp"
#include "Common/Component/ComponentModel.hpp"
#include "Common/Physics/PhysicsWorld.hpp"

GameState_Singleplayer::GameState_Singleplayer()
{
	this->scene_root = GameObjectManager::createGameObject();
	this->scene_root->addComponent<PhysicsWorld>();

	GameObject* ship = GameObjectManager::createGameObject();
	this->scene_root->addChild(ship);

	ship->addComponent<RigidBody>();
	ship->addComponent<ComponentModel>("res/models/Cobra/Hull.obj", "res/textures/1K_Grid.png", "Textured", "Textured_Lighting", "Textured_Shadow");

	GameObject* camera = GameObjectManager::createGameObject();
	this->scene_root->addChild(camera);
	camera->addComponent<Camera>();
	scene_camera = camera->getComponent<Camera>();
	camera->setLocalTransform(Transform(vector3D(0.0, 3.0, -8.0)));
}

GameState_Singleplayer::~GameState_Singleplayer()
{
	delete this->scene_root;
}

void GameState_Singleplayer::update(Client* client, double delta_time)
{
	this->scene_root->update(delta_time);

	client->renderingManager->renderScene(scene_root, scene_camera);
}