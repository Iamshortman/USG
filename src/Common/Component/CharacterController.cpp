#include "CharacterController.hpp"

#include "Common/Transform.hpp"
#include "Common/World/World.hpp"

#include "Common/Logger/Logger.hpp"

CharacterController::CharacterController(Entity* entity)
	:ComponentEntity(entity)
{
}

CharacterController::~CharacterController()
{
}

void CharacterController::update(double delta_time)
{
	this->checkOnGround();
	this->doMovement(delta_time);
	this->doRotation(delta_time);
	this->doHeadRotation(delta_time);
}

void CharacterController::setHead(Node* head)
{
	this->head_node = head;
}

void CharacterController::checkOnGround()
{
	Transform transform = this->parent_entity->getWorldTransform();
	SingleRayTestResult ray = this->parent_entity->getWorld()->singleRayTestNotMe(transform.getPosition(), transform.getPosition() + (transform.getUp() * -this->ray_distance), this->parent_entity);
	if (ray.hasHit)
	{
		vector3D hit_vector = glm::normalize(ray.hitNormal);
		double dot_product = glm::dot(transform.getUp(), hit_vector);

		//printf("Dot: %lf\n", dot_product);

		this->on_ground = true;
		return;
	}

	this->on_ground = false;
}

void CharacterController::doMovement(double delta_time)
{
	const double movement_speed = 5.0;

	const double ground_acceleration = 25.0;
	const double air_acceleration = 2.0;

	const double jump_speed = 6.0;

	RigidBody* rigid_body = this->parent_entity->getRigidBody();
	Transform transform = this->parent_entity->getWorldTransform();
	vector3D character_velocity = rigid_body->getLinearVelocity();

	{ //Foward
		double currentForwardVelocity = glm::dot(transform.getForward(), character_velocity);
		double desiredForwardVelocity = this->linear_input.z * movement_speed;
		double difference = abs(currentForwardVelocity - desiredForwardVelocity);
		if (currentForwardVelocity < desiredForwardVelocity)
		{
			if (this->on_ground || this->linear_input.z > 0.0)
			{
				double velocitytoAdd = delta_time * (this->on_ground ? ground_acceleration : air_acceleration);
				if (velocitytoAdd >= difference)
				{
					velocitytoAdd = difference;
				}

				character_velocity += transform.getForward() * velocitytoAdd;
			}
		}
		else if (currentForwardVelocity > desiredForwardVelocity)
		{
			if (this->on_ground || this->linear_input.z < 0.0)
			{
				double velocitytoAdd = delta_time * (this->on_ground ? ground_acceleration : air_acceleration);
				if (velocitytoAdd >= difference)
				{
					velocitytoAdd = difference;
				}

				character_velocity += -transform.getForward() * velocitytoAdd;
			}
		}
	}

	{ //Left
		double currentForwardVelocity = glm::dot(transform.getLeft(), character_velocity);
		double desiredForwardVelocity = this->linear_input.x * movement_speed;
		double difference = abs(currentForwardVelocity - desiredForwardVelocity);
		if (currentForwardVelocity < desiredForwardVelocity)
		{
			if (this->on_ground || this->linear_input.x > 0.0)
			{
				double velocitytoAdd = delta_time * (this->on_ground ? ground_acceleration : air_acceleration);
				if (velocitytoAdd >= difference)
				{
					velocitytoAdd = difference;
				}

				character_velocity += transform.getLeft() * velocitytoAdd;
			}
		}
		else if (currentForwardVelocity > desiredForwardVelocity)
		{
			if (this->on_ground || this->linear_input.x < 0.0)
			{
				double velocitytoAdd = delta_time * (this->on_ground ? ground_acceleration : air_acceleration);
				if (velocitytoAdd >= difference)
				{
					velocitytoAdd = difference;
				}

				character_velocity += -transform.getLeft() * velocitytoAdd;
			}
		}
	}

	static bool can_jump = false;
	if (this->on_ground)
	{
		if (this->jump)
		{
			if (can_jump)
			{
				character_velocity += transform.getUp() * jump_speed;
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

	rigid_body->setLinearVelocity(character_velocity);
}

void CharacterController::doRotation(double delta_time)
{
	const double turn_speed = 1.5;

	RigidBody* rigid_body = this->parent_entity->getRigidBody();
	Transform transform = this->parent_entity->getLocalTransform();
	vector3D character_velocity = glm::inverse(transform.getOrientation()) * rigid_body->getLinearVelocity();

	quaternionD orientation = transform.getOrientation();
	orientation = glm::angleAxis(turn_speed * this->angular_input.y * (M_PI * 2.0) * delta_time, transform.getUp()) * orientation;
	transform.setOrientation(orientation);

	if (this->on_ground)
	{
		character_velocity = transform.getOrientation() * character_velocity;
		rigid_body->setLinearVelocity(character_velocity);
	}

	this->parent_entity->setLocalTransform(transform);
}

double clamp(double value, double min, double max)
{
	if (value < min)
	{
		return min;
	}
	else if(value > max)
	{
		return max;
	}

	return value;
}


void CharacterController::doHeadRotation(double delta_time)
{
	const double turn_speed = 1.5;

	if (this->head_node != nullptr)
	{
		Transform head_transform = this->head_node->getLocalTransform();

		this->head_rotation += turn_speed * this->angular_input.x * (M_PI * 2.0) * delta_time;
		this->head_rotation = clamp(this->head_rotation, -(M_PI / 2.0), (M_PI / 2.0));

		head_transform.setOrientation(glm::angleAxis(this->head_rotation, head_transform.getLeft()));
		this->head_node->setLocalTransform(head_transform);
	}
}
