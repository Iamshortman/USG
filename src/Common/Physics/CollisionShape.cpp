#include "CollisionShape.hpp"

#include "Common/GameObject.hpp"
#include "Common/Physics/Bullet_Include.hpp"
#include "Common/Physics/RigidBody.hpp"
#include "Common/Physics/PhysicsWorld.hpp"

CollisionShape::CollisionShape()
{
}

CollisionShape::~CollisionShape()
{
}

void CollisionShape::enable()
{
	if (!this->enabled)
	{
		GameObject* game_object = this->parent->findParentWithFirst_StopIfSecond<RigidBody, PhysicsWorld>();
		if (game_object != nullptr)
		{
			this->index = game_object->getComponent<RigidBody>()->addChildShape(this);
			Component::enable();
		}
	}
}

void CollisionShape::disable()
{
	if (this->enabled)
	{
		GameObject* game_object = this->parent->findParentWithFirst_StopIfSecond<RigidBody, PhysicsWorld>();
		if (game_object != nullptr)
		{
			game_object->getComponent<RigidBody>()->removeChildShape(this->index);
			this->index = -1;
		}

		Component::disable();
	}
}

void CollisionShape::setBox(vector3D half_length)
{
	if (this->shape != nullptr)
	{
		delete this->shape;
	}

	this->shape_type = Box;
	this->shape = new btBoxShape(toBtVec3(half_length));
}

void CollisionShape::setCapsule(double radius, double height)
{
	if (this->shape != nullptr)
	{
		delete this->shape;
	}

	this->shape_type = Capsule;
	this->shape = new btCapsuleShape(radius, height);
}

btCollisionShape* CollisionShape::getShape()
{
	return this->shape;
}
