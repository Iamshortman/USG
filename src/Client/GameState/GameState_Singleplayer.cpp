#include "Client/GameState/GameState_Singleplayer.hpp"
#include "Client/Client.hpp"

#include "Common/Entity/Entity.hpp"
#include "Common/Entity/EntityManager.hpp"

#include "Common/World/WorldManager.hpp"

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

#include <algorithm>

NodeEntity* build_big_ship()
{
	NodeEntity* node_entity = EntityManager::getInstance()->createNodeEntity();
	node_entity->addRigidBody();

	Node* hull = new Node();
	node_entity->addChild(hull);
	hull->addNodeComponent<Model>("res/Cube_Ship/Hull.obj", "res/Cube_Ship/Hull.png", "res/shaders/Textured", "res/shaders/Shadow");
	hull->addNodeComponent<CollisionShape>(CollisionShapeType::ConvexMesh, "res/Cube_Ship/Hull.obj");
	hull->addNodeComponent<Mass>(7900000.0);

	Node* bridge = new Node();
	node_entity->addChild(bridge);
	bridge->addNodeComponent<Model>("res/Cube_Ship/Bridge.obj", "res/Cube_Ship/Bridge.png", "res/shaders/Textured", "res/shaders/Shadow");
	bridge->addNodeComponent<CollisionShape>(CollisionShapeType::ConvexMesh, "res/Cube_Ship/Bridge.obj");
	bridge->addNodeComponent<Mass>(100000.0);

	//node_entity->getRigidBody()->setMass(8000000.0);
	//node_entity->getRigidBody()->setInertiaTensor(vector3D(1000000000.0));
	node_entity->getRigidBody()->getRigidBody()->forceActivationState(DISABLE_DEACTIVATION);
	//node_entity->addComponent<EntityHealth>(1000.0);

	node_entity->setSubWorld(WorldManager::getInstance()->createWorld());
	node_entity->getSubWorld()->setGravity(vector3D(0.0, -9.8, 0.0));

	Entity* interior = EntityManager::getInstance()->createEntity();
	interior->addRigidBody();
	interior->getRigidBody()->setMass(0.0);
	interior->addToWorld(node_entity->getSubWorld());
	interior->addNodeComponent<Model>("res/Cube_Ship/Interior.obj", "res/Cube_Ship/Interior.png", "res/shaders/Textured", "res/shaders/Shadow");
	interior->addNodeComponent<CollisionShape>(CollisionShapeType::ConcaveMesh, "res/Cube_Ship/Interior.obj");

	Entity* interior_bridge = EntityManager::getInstance()->createEntity();
	interior_bridge->addRigidBody();
	interior_bridge->getRigidBody()->setMass(0.0);
	interior_bridge->addToWorld(node_entity->getSubWorld());
	interior_bridge->addNodeComponent<Model>("res/Cube_Ship/Interior_Bridge.obj", "res/Cube_Ship/Interior_Bridge.png", "res/shaders/Textured", "res/shaders/Shadow");
	interior_bridge->addNodeComponent<CollisionShape>(CollisionShapeType::ConcaveMesh, "res/Cube_Ship/Interior_Bridge.obj");

	return node_entity;
}

GameState_Singleplayer::GameState_Singleplayer()
{
	this->pid = new QuaternionPidController(1.0, 0.0, -0.075, -1.0, 1.0, 1.0);

	this->world = WorldManager::getInstance()->createWorld();

	ai_ship = (NodeEntity*)EntityConstructor::buildEntityFromJson("res/json/Ship.json");
	ai_ship->getRigidBody()->setInertiaTensor(vector3D(100000.0));

	ai_ship->addToWorld(this->world);
	ai_ship->setLocalTransform(Transform(vector3D(0.0, 0.0, 0.0)));
	//ai_ship->removeComponent<ShipFlightController>();

	this->ship = (NodeEntity*)EntityConstructor::buildEntityFromJson("res/json/Ship.json");
	this->ship->addToWorld(this->world);
	this->ship->setLocalTransform(Transform(vector3D(0.0, 0.0, -50.0)));

	Node* head = new Node();
	this->ship->addChild(head);
	this->ship->active_camera = head->addNodeComponent<Camera>();
	head->setLocalTransform(vector3D(0.0, 1.1, -2.0));


	character = EntityManager::getInstance()->createEntity();
	character->active_camera = character->addNodeComponent<Camera>();
	character->addComponent<DebugCamera>(5.0, 0.5);
	character->addToWorld(this->world);
	character->setLocalTransform(Transform(vector3D(5.0, 0.0, 0.0), quaternionD(0.0, 0.0, 1.0, 0.0)));

	this->player_controller.setPlayerEntity(character);
}

GameState_Singleplayer::~GameState_Singleplayer()
{
	WorldManager::getInstance()->destroyWorld(this->world->worldId);
	delete pid;
}

void rotateTowards(NodeEntity* entity, QuaternionPidController* pid, Transform target, double delta_time)
{
	/*const double input = 1.0;

	if (entity->hasComponent<ShipFlightController>())
	{
		ShipFlightController* controller = entity->getComponent<ShipFlightController>();
		RigidBody* rigid_body = entity->getRigidBody();

		Transform transform = entity->getWorldTransform();
		Transform relative_transform = target.untransformBy(transform);

		vector3D forward = glm::normalize(relative_transform.getPosition());
		vector3D relative_pos = relative_transform.getPosition();

		//Pitch
		if (true)
		{
			double angle = atan2(forward.y, forward.z);

			double output = 0.0;

			//PID section
			if (true)
			{
				//Consts
				double proptional_const = 10.0; //Proptional Constant
				double intergral_const = 0.0; //Integral Constant
				double derivative_const = 23.0; //Derivative Constant
				double error = 0.0 - angle;

				//TODO not statics
				static double prev_error = 0.0;
				static double intergral_error = 0.0;

				intergral_error += error;
				double derivative_error = error - prev_error;


				output = (proptional_const * error) + (intergral_const * intergral_error * delta_time) + ((derivative_const * derivative_error) / delta_time);

				//printf("Output: %f\n", output);

				if (output > 1.0)
				{
					output = 1.0;
				}
				else if (output < -1.0)
				{
					output = -1.0;

				}

				prev_error = error;
			}

			controller->angular_input.x = output;
		}

		//Yaw
		if (true)
		{
			double angle = atan2(forward.x, forward.y);

			double output = 0.0;

			//PID section
			if (true)
			{
				//Consts
				double proptional_const = 10.0; //Proptional Constant
				double intergral_const = 0.0; //Integral Constant
				double derivative_const = 60.0; //Derivative Constant
				double error = 0.0 - angle;

				//TODO not statics
				static double prev_error = 0.0;
				static double intergral_error = 0.0;

				intergral_error += error;
				double derivative_error = error - prev_error;


				output = (proptional_const * error) + (intergral_const * intergral_error * delta_time) + ((derivative_const * derivative_error) / delta_time);

				//printf("Output: %f\n", output);

				if (output > 1.0)
				{
					output = 1.0;
				}
				else if (output < -1.0)
				{
					output = -1.0;

				}

				prev_error = error;
			}

			controller->angular_input.y = -output;
		}

	}*/

	Transform transform = entity->getWorldTransform();
	quaternionD current_orientation = transform.getOrientation();
	quaternionD desired_orientation = glm::quatLookAt(glm::normalize(transform.position - target.position), transform.getUp());

	RigidBody* rigid_body = entity->getRigidBody();
	vector3D input = pid->calculate(current_orientation, desired_orientation, rigid_body->getAngularVelocity(), delta_time);
	//rigid_body->applyTorqueImpulse(input);
	printf("Input: %s\n", glm::to_string(input).c_str());

	if (entity->hasComponent<ShipFlightController>())
	{
		entity->getComponent<ShipFlightController>()->angular_input = input;
	}
}

void GameState_Singleplayer::update(Client* client, double delta_time)
{
	this->player_controller.applyUserInput(delta_time);

	//AI: AIM AT TARGET
	rotateTowards(this->ai_ship, this->pid, this->character->getWorldTransform(), delta_time);

	WorldManager::getInstance()->update(delta_time);
	EntityManager::getInstance()->update();

	client->renderingManager->Render(this->player_controller.getWorld(), this->player_controller.getCamera());
}