#include "CharacterController.hpp"

#include "Common/Transform.hpp"
#include "Common/World/World.hpp"

CharacterController::CharacterController(Entity* entity)
	:ComponentEntity(entity)
{
}

CharacterController::~CharacterController()
{
}

void CharacterController::update(double delta_time)
{
	RigidBody* rigid_body = this->parent_entity->getRigidBody();
	Transform transform = this->parent_entity->getWorldTransform();
	vector3D character_velocity = glm::inverse(transform.getOrientation()) * rigid_body->getLinearVelocity();
	vector3D plane_velocity = vector3D(character_velocity.x, 0.0, character_velocity.z);

	this->on_ground = checkOnGround();

	rigid_body->applyCentralImpulse(transform.getOrientation() * this->linear_input * (this->on_ground ? this->move_force : this->inair_move_force)); //TODO use non-impulse forces?

	static bool can_jump = false;

	if (this->on_ground)
	{
		vector3D break_force = -plane_velocity * this->brake_force;
		rigid_body->applyCentralImpulse(transform.getOrientation() * break_force);

		if (this->jump)
		{
			if (can_jump)
			{
				rigid_body->applyCentralImpulse(transform.getUp() * this->jump_force);
				can_jump = false;
			}
		}
		else
		{
			can_jump = true;
		}
	}
	else
	{
		can_jump = false;
	}

	//printf("Ground: %s\n", this->on_ground ? "true" : "false");
}

bool CharacterController::checkOnGround()
{
	Transform transform = this->parent_entity->getWorldTransform();
	SingleRayTestResult ray = this->parent_entity->getWorld()->singleRayTestNotMe(transform.getPosition(), transform.getPosition() + (transform.getUp() * -this->ray_distance), this->parent_entity);
	if (ray.hasHit)
	{
		vector3D hit_vector = glm::normalize(ray.hitNormal);
		double dot_product = glm::dot(transform.getUp(), hit_vector);

		//printf("Dot: %lf\n", dot_product);

		return true;
	}

	return false;
}
