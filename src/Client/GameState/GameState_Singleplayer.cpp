#include "Client/GameState/GameState.hpp"
#include "Client/Client.hpp"

#include "Common/GameObject.hpp"
#include "Common/GameObjectManager.hpp"

#include "Client/Component/DebugCamera.hpp"
#include "Client/Rendering/Camera.hpp"
#include "Common/Component/ComponentModel.hpp"

#include "Common/Physics/PhysicsWorld.hpp"

GameState_Singleplayer::GameState_Singleplayer()
{
	this->scene_root = GameObjectManager::getInstance()->createGameObject();
	this->scene_root->addComponent<PhysicsWorld>();

	this->scene_root->addChild(GameObjectManager::getInstance()->createGameObjectFromJson("res/json/test.json"));



	GameObject* camera = GameObjectManager::getInstance()->createGameObject();
	camera->addComponent<Camera>();
	camera->addComponent<DebugCamera>(5.0, 1.0);
	scene_camera = camera->getComponent<Camera>();
	camera->setLocalTransform(Transform(vector3D(0.0, 2.0, -8.0)));
	this->scene_root->addChild(camera);
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