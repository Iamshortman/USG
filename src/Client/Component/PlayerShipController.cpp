#include "PlayerShipController.hpp"

#include "Common/Entity/Entity.hpp"
#include "Common/Component/ComponentShipFlight.hpp"

PlayerShipController::PlayerShipController(Entity* entity)
	:ComponentEntity(entity),
	retro_burn_controller(1.75, 0.0, -0.075, -1.0, 1.0, 1.0),
	velocity_limit_angular(6.0, 0.0, 0.0, -1.0, 1.0, 1.0)
{
}

PlayerShipController::~PlayerShipController()
{
}

void PlayerShipController::update(double delta_time)
{
	if (this->parent_entity->hasComponent<ShipFlightController>())
	{
		ShipFlightController* controller = this->parent_entity->getComponent<ShipFlightController>();

		//TODO Linear Modes
		controller->linear_input = this->player_input_linear;

		if (this->rotation_mode == RotationalFlightMode::Normal)
		{
			if (retro_burn_mode_input == true)
			{
				this->rotation_mode = RotationalFlightMode::Retro_Burn;
			}
		}
		else if (this->rotation_mode == RotationalFlightMode::Retro_Burn)
		{
			if (this->player_input_angular.x != 0.0 || this->player_input_angular.y != 0.0)
			{
				this->rotation_mode = RotationalFlightMode::Normal;
			}
		}

		if (this->rotation_mode == RotationalFlightMode::Normal)
		{
			Transform transform = this->parent_entity->getLocalTransform();
			RigidBody* rigid_body = this->parent_entity->getRigidBody();
			if (rigid_body != nullptr)
			{
				vector3D local_angular_velocity = glm::inverse(transform.getOrientation()) * rigid_body->getAngularVelocity();
				vector3D angular_input = this->velocity_limit_angular.calculate(this->player_input_angular * this->max_angular_speed, local_angular_velocity, vector3D(0.0), delta_time);
				controller->angular_input = angular_input;
			}
		}
		else if(this->rotation_mode == RotationalFlightMode::Retro_Burn)
		{
			RigidBody* rigid_body = this->parent_entity->getRigidBody();
			if (rigid_body != nullptr)
			{
				Transform transform = this->parent_entity->getLocalTransform();
				vector3D retro_direction = rigid_body->getLinearVelocity();
				if (retro_direction == vector3D(0.0))
				{
					//Keep Forward
					retro_direction = transform.getForward();
				}

				vector3D ai_input = this->retro_burn_controller.calculate(transform.getOrientation(), glm::quatLookAt(glm::normalize(retro_direction), transform.getUp()), rigid_body->getAngularVelocity(), delta_time);
				controller->angular_input = ai_input;

				//Use Roll Override
				if (this->player_input_angular.z != 0.0)
				{
					controller->angular_input.z = this->player_input_angular.z;
				}
			}
		}


	}
}
