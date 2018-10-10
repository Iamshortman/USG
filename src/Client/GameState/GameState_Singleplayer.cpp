#include "GameState_Singleplayer.hpp"
#include "Client/Client.hpp"

#include "Common/Rendering/Model.hpp"

#include "Common/Physics/PhysicsSystem.hpp"
#include "Common/World/NodeSystem.hpp"
#include "Common/World/WorldSystem.hpp"
#include "Common/Component/Projectile.hpp"
#include "Common/Component/TimeToLive.hpp"
#include "Common/Component/FlightController.hpp"
#include "Common/Component/Seat.hpp"

#include "Client/RenderingSystem.hpp"
#include "Client/Input/InputManager.hpp"

#include "Client/Component/DebugCamera.hpp"

GameState_Singleplayer::GameState_Singleplayer()
{
	this->ecs_system.systems.add<PhysicsSystem>();
	this->ecs_system.systems.add<NodeSystem>();
	this->ecs_system.systems.add<WorldSystem>();
	this->ecs_system.systems.add<DebugCameraSystem>();
	this->ecs_system.systems.add<ProjectileSystem>();
	this->ecs_system.systems.add<TimeToLiveSystem>();
	this->ecs_system.systems.add<FlightControllerSystem>();
	this->ecs_system.systems.add<SeatSystem>();

	this->ecs_system.systems.add<RenderingSystem>();
	this->ecs_system.systems.configure();



	Entity root = this->ecs_system.entities.create();
	root.assign<WorldHost>(1);

	Entity camera = this->ecs_system.entities.create();
	camera.assign<Transform>(vector3D(0.0, 2.0, -10.0));
	camera.assign<World>(1);
	camera.assign<Camera>();
	camera.assign<DebugCamera>(15.0, 0.5);

	this->entity_with_seat = this->ecs_system.entities.create();
	this->entity_with_seat.assign<Transform>();
	this->entity_with_seat.assign<World>(1);
	this->entity_with_seat.assign<NodeHost>();
	this->entity_with_seat.assign<Model>("res/models/cube.obj", "res/textures/Red.png", "res/shaders/Textured", "res/shaders/Shadow");
	this->entity_with_seat.assign<RigidBody>();
	this->entity_with_seat.assign<CollisionShape>(new reactphysics3d::BoxShape(reactphysics3d::Vector3(0.5, 0.5, 0.5)), 10000.0);
	this->entity_with_seat.assign<ProjectileLauncher>();

	Entity seat = this->ecs_system.entities.create();
	seat.assign<Seat>("seat");
	seat.assign<Node>(this->entity_with_seat, Transform(vector3D(0.0, 1.0, 0.0)));

	this->entity = this->ecs_system.entities.create();
	this->entity.assign<Transform>(vector3D(0.0, 10.0, 0.0));
	this->entity.assign<World>(1);
	this->entity.assign<Model>("res/models/cube.obj", "res/textures/Purple.png", "res/shaders/Textured", "res/shaders/Shadow");
	this->entity.assign<RigidBody>();
	this->entity.component<RigidBody>()->setLinearVelocity(vector3D(0.0, 0.0, 0.0));
	this->entity.assign<CollisionShape>(new reactphysics3d::BoxShape(reactphysics3d::Vector3(0.5, 0.5, 0.5)), 10000.0);
	this->entity.assign<FlightController>();
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

	if (InputManager::getInstance()->getButtonPressed("Debug_FlightAssist"))
	{
		this->ecs_system.events.emit<AddToSeatEvent>(this->entity, this->entity_with_seat, "seat");
	}

	this->ecs_system.systems.update_all((entityx::TimeDelta) delta_time);
	//client->renderingManager->renderScene();
}