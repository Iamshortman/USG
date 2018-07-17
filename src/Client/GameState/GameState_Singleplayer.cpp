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

Entity create_cube_ship(EntityX& ecs)
{
	Entity entity = ecs.entities.create();
	entity.assign<Model>("res/Cube_Ship/Hull.obj", "res/Cube_Ship/Hull.png", "res/shaders/Textured", "");
	entity.assign<Transform>(vector3D(0.0, 0.0, 100.0));
	entity.assign<World>(1);
	entity.assign<WorldHost>(2)->physics_world->dynamicsWorld->setGravity(btVector3(0.0, -9.8, 0.0));
	entity.assign<RigidBody>()->setMass(10000000.0);
	entity.component<RigidBody>()->setInertiaTensor(vector3D(10000000.0));
	entity.assign<MuiltiCollisionShape>();
	entity.assign_from_copy<CollisionShape>(CollisionShapes::createConvexMesh("res/Cube_Ship/Hull.obj"));

	Entity bridge = ecs.entities.create();
	bridge.assign<Model>("res/Cube_Ship/Bridge.obj", "res/Cube_Ship/Bridge.png", "res/shaders/Textured", "");
	bridge.assign<Node>(entity, Transform(vector3D(0.0)));
	bridge.assign_from_copy<CollisionShape>(CollisionShapes::createConvexMesh("res/Cube_Ship/Bridge.obj"));

	if (true)
	{
		Entity interior = ecs.entities.create();
		interior.assign<Model>("res/Cube_Ship/Interior.obj", "res/Cube_Ship/Interior.png", "res/shaders/Textured", "");
		interior.assign<Transform>(vector3D(0.0));
		interior.assign<World>(2);
		interior.assign<RigidBody>()->setMass(0.0);
		interior.assign_from_copy<CollisionShape>(CollisionShapes::createConcaveMesh("res/Cube_Ship/Interior.obj"));


		Entity interior_bridge = ecs.entities.create();
		interior_bridge.assign<Model>("res/Cube_Ship/Interior_Bridge.obj", "res/Cube_Ship/Interior_Bridge.png", "res/shaders/Textured", "");
		interior_bridge.assign<Transform>(vector3D(0.0));
		interior_bridge.assign<World>(2);
		interior_bridge.assign<RigidBody>()->setMass(0.0);
		interior_bridge.assign_from_copy<CollisionShape>(CollisionShapes::createConcaveMesh("res/Cube_Ship/Interior_Bridge.obj"));
	}

	return entity;
}


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
	root.assign<WorldHost>(1);

	entity = this->ecs_system.entities.create();
	entity.assign<Transform>(vector3D(0.0));
	entity.assign<World>(1);
	entity.assign<Model>("res/models/A-Wing/a-wing-body.obj", "res/textures/A-Wing/a-wing-body.png", "res/shaders/Textured", "");
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
	camera.assign<World>(1);
	camera.assign<Camera>();
	camera.assign<DebugCamera>(15.0, 0.5);

	//create_cube_ship(this->ecs_system);
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