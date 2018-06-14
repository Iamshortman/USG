#include "Client/GameState/GameState_Singleplayer.hpp"
#include "Client/Client.hpp"

#include "Common/Physics/PhysicsSystem.hpp"

#include "Common/Physics/RigidBody.hpp"
#include "Common/Component/ComponentModel.hpp"

GameState_Singleplayer::GameState_Singleplayer()
{
	this->ecs_system.systems.add<PhysicsSystem>();
	this->ecs_system.systems.configure();

	Entity entity = this->ecs_system.entities.create();
	entity.assign<Transform>(vector3D(0.0, -2.0, 10.0));
	entity.assign<ComponentModel>("res/models/A-Wing/a-wing-body.obj", "res/textures/A-Wing/a-wing-body.png", "res/shaders/Textured", "", "");
	entity.assign<RigidBody>();
}

GameState_Singleplayer::~GameState_Singleplayer()
{
	this->ecs_system.entities.reset();
}

void GameState_Singleplayer::update(Client* client, double delta_time)
{
	this->ecs_system.systems.update_all((entityx::TimeDelta) delta_time);

	client->renderingManager->renderScene(this->ecs_system);
}