#include "Client/GameState/GameState_Singleplayer.hpp"
#include "Client/Client.hpp"

#include "Client/Rendering/RenderingSystem.hpp"

#include "Common/Physics/PhysicsSystem.hpp"
#include "Client/Component/DebugCamera.hpp"

#include "Common/Physics/SingleRigidBody.hpp"
#include "Common/WorldSystem.hpp"
#include "Common/Rendering/Model.hpp"

GameState_Singleplayer::GameState_Singleplayer()
{
	this->ecs_system.systems.add<PhysicsSystem>();
	this->ecs_system.systems.add<RenderingSystem>();
	this->ecs_system.systems.add<WorldSystem>();
	this->ecs_system.systems.add<DebugCameraSystem>();
	this->ecs_system.systems.configure();

	Entity root = this->ecs_system.entities.create();
	root.assign<WorldHost>(0);

	entity = this->ecs_system.entities.create();
	entity.assign<Transform>(vector3D(0.0, -2.0, 10.0));
	entity.assign<World>(0);
	entity.assign<Model>("res/models/A-Wing/a-wing-body.obj", "res/textures/A-Wing/a-wing-body.png", "res/shaders/Textured", "");
	entity.assign<SingleRigidBody>();
	entity.assign<CollisionShape>()->setConvexMesh("res/models/A-Wing/a-wing-body.obj");
	entity.component<SingleRigidBody>()->Activate(true);
	entity.component<SingleRigidBody>()->applyTorqueImpulse(vector3D(2.0, 0.0, 0.0));
	entity.assign<NodeHost>();

	Entity child = this->ecs_system.entities.create();
	child.assign<Node>(entity, Transform(vector3D(0.0, 2.0, 0.0)));
	child.assign<Model>("res/models/cube.obj", "res/textures/Red.png", "res/shaders/Textured", "");
	child.assign<NodeHost>();

	Entity child1 = this->ecs_system.entities.create();
	child1.assign<Node>(child, Transform(vector3D(0.0, 2.0, 3.0)));
	child1.assign<Model>("res/models/cube.obj", "res/textures/Blue.png", "res/shaders/Textured", "");
	child1.assign<NodeHost>();



	Entity camera = this->ecs_system.entities.create();
	camera.assign<Transform>();
	camera.assign<World>(0);
	camera.assign<Camera>();
	camera.assign<DebugCamera>(5.0, 0.5);
}

GameState_Singleplayer::~GameState_Singleplayer()
{
	this->ecs_system.entities.reset();
}

void GameState_Singleplayer::update(Client* client, double delta_time)
{
	this->ecs_system.systems.update_all((entityx::TimeDelta) delta_time);
	client->renderingManager->renderScene();
}