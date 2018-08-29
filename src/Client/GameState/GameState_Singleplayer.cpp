#include "Client/GameState/GameState_Singleplayer.hpp"
#include "Client/Client.hpp"

#include "Common/Rendering/Model.hpp"

#include "Common/Physics/RigidBody.hpp"

#include "Common/Physics/PhysicsSystem.hpp"
#include "Common/World/NodeSystem.hpp"
#include "Common/World/WorldSystem.hpp"
#include "Client/Component/DebugCamera.hpp"
#include "Common/Component/Projectile.hpp"
#include "Common/Component/TimeToLive.hpp"
#include "Common/Component/FlightController.hpp"

#include "Client/RenderingSystem.hpp"

#include "Client/Input/InputManager.hpp"

GameState_Singleplayer::GameState_Singleplayer()
{
	this->ecs_system.systems.add<PhysicsSystem>();
	this->ecs_system.systems.add<NodeSystem>();
	this->ecs_system.systems.add<WorldSystem>();
	this->ecs_system.systems.add<DebugCameraSystem>();
	this->ecs_system.systems.add<ProjectileSystem>();
	this->ecs_system.systems.add<TimeToLiveSystem>();
	this->ecs_system.systems.add<FlightControllerSystem>();

	this->ecs_system.systems.add<RenderingSystem>();
	this->ecs_system.systems.configure();

	Entity root = this->ecs_system.entities.create();
	root.assign<WorldHost>(1);

	entity = this->ecs_system.entities.create();
	entity.assign<Transform>(vector3D(0.0));
	entity.assign<World>(1);
	entity.assign<Model>("res/models/A-Wing/a-wing-body.obj", "res/textures/A-Wing/a-wing-body.png", "res/shaders/Textured", "res/shaders/Shadow");
	entity.assign<RigidBody>()->setMass(40000.0);
	entity.component<RigidBody>()->setInertiaTensor(vector3D(10000.0));
	entity.assign<MuiltiCollisionShape>();
	entity.assign_from_copy<CollisionShape>(CollisionShapes::createConvexMesh("res/models/A-Wing/a-wing-body.obj"));
	entity.assign<NodeHost>();
	entity.assign<FlightController>()->linear_input.z = 1.0;

	Entity gun1 = this->ecs_system.entities.create();
	gun1.assign<Node>(entity, Transform(vector3D(3.0, 0.0, -0.5)));
	gun1.assign<Model>("res/models/cube.obj", "res/textures/Red.png", "res/shaders/Textured", "res/shaders/Shadow");
	gun1.assign<ProjectileLauncher>();

	Entity camera = this->ecs_system.entities.create();
	//camera.assign<Transform>(vector3D(0.0, 2.0, -10.0));
	//camera.assign<World>(1);
	camera.assign<Node>(entity, Transform(vector3D(0.0, 2.0, -10.0)) );
	camera.assign<Camera>();
	camera.assign<DebugCamera>(15.0, 0.5);
}

GameState_Singleplayer::~GameState_Singleplayer()
{
	this->ecs_system.entities.reset();
}

void GameState_Singleplayer::update(Client* client, double delta_time)
{
	if (entity.has_component<FlightController>())
	{
		ComponentHandle<FlightController> flight_controller = entity.component<FlightController>();

		flight_controller->linear_input.x = InputManager::getInstance()->getButtonAxisCombo("Debug_LeftRight", "Debug_Left", "Debug_Right");
		flight_controller->linear_input.y = InputManager::getInstance()->getButtonAxisCombo("Debug_UpDown", "Debug_Up", "Debug_Down");
		flight_controller->linear_input.z = InputManager::getInstance()->getButtonAxisCombo("Debug_ForwardBackward", "Debug_Forward", "Debug_Backward");

		flight_controller->angular_input.x = InputManager::getInstance()->getButtonAxisCombo("Debug_Pitch", "Debug_PitchUp", "Debug_PitchDown");
		flight_controller->angular_input.y = InputManager::getInstance()->getButtonAxisCombo("Debug_Yaw", "Debug_YawLeft", "Debug_YawRight");
		flight_controller->angular_input.z = InputManager::getInstance()->getButtonAxisCombo("Debug_Roll", "Debug_RollLeft", "Debug_RollRight");

		flight_controller->flight_assist = InputManager::getInstance()->getButtonDown("Debug_FlightAssist");
	}

	this->ecs_system.systems.update_all((entityx::TimeDelta) delta_time);
	//client->renderingManager->renderScene();
}