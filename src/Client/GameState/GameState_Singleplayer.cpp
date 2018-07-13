#include "Client/GameState/GameState_Singleplayer.hpp"
#include "Client/Client.hpp"

#include "Common/Rendering/Model.hpp"

#include "Common/Physics/RigidBody.hpp"


#include "Common/Physics/PhysicsSystem.hpp"
#include "Common/WorldSystem.hpp"
#include "Client/Component/DebugCamera.hpp"
#include "Common/ProjectileSystem.hpp"
#include "Common/TimeToLiveSystem.hpp"

#include "Client/Rendering/RenderingSystem.hpp"

GameState_Singleplayer::GameState_Singleplayer()
{
	this->ecs_system.systems.add<PhysicsSystem>();
	this->ecs_system.systems.add<WorldSystem>();
	this->ecs_system.systems.add<DebugCameraSystem>();
	this->ecs_system.systems.add<ProjectileSystem>();
	this->ecs_system.systems.add<TimeToLiveSystem>();

	this->ecs_system.systems.add<RenderingSystem>();
	this->ecs_system.systems.configure();

	Entity root = this->ecs_system.entities.create();
	root.assign<WorldHost>(0);

	entity = this->ecs_system.entities.create();
	entity.assign<Transform>(vector3D(0.0));
	entity.assign<World>(0);
	//entity.assign<Model>("res/models/A-Wing/a-wing-body.obj", "res/textures/A-Wing/a-wing-body.png", "res/shaders/Textured", "");
	entity.assign<Model>("res/Cube_Ship/Hull.obj", "res/Cube_Ship/Hull.png", "res/shaders/Textured", "");
	entity.assign<RigidBody>();
	entity.assign<MuiltiCollisionShape>();
	entity.assign_from_copy<CollisionShape>(CollisionShapes::createConvexMesh("res/models/A-Wing/a-wing-body.obj"));
	entity.assign<NodeHost>();

	Entity gun1 = this->ecs_system.entities.create();
	gun1.assign<Node>(entity, Transform(vector3D(2.0, 0.0, -0.5)));
	gun1.assign<Model>("res/models/cube.obj", "res/textures/Red.png", "res/shaders/Textured", "");
	gun1.assign<ProjectileLauncher>();

	Entity camera = this->ecs_system.entities.create();
	camera.assign<Transform>(vector3D(0.0, 2.0, -10.0));
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