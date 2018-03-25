#include "Client/GameState/GameState.hpp"
#include "Client/Client.hpp"

#include "Common/GameObject.hpp"
#include "Common/Component/ComponentModel.hpp"
#include "Common/Physics/PhysicsWorld.hpp"

GameState_Singleplayer::GameState_Singleplayer()
{
	this->scene_root = new GameObject(1);
	this->scene_root->addComponent<PhysicsWorld>();

	GameObject* ship = new GameObject(2);
	this->scene_root->addChild(ship);

	ship->addComponent<RigidBody>();
	ship->addComponent<ComponentModel>("res/models/Cobra/Hull.obj", "res/textures/1K_Grid.png", "Textured", "Textured_Lighting", "Textured_Shadow");
}

GameState_Singleplayer::~GameState_Singleplayer()
{
	delete this->scene_root;
}

void GameState_Singleplayer::update(Client* client, double delta_time)
{
	this->scene_root->update(delta_time);

	Camera camera;
	camera.setCameraPos(vector3D(0.0, 0.0, -10.0));

	client->renderingManager->renderScene(scene_root, &camera);
}