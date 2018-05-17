#include "CollisionShape.hpp"

#include "Common/GameObject.hpp"
#include "Common/Physics/Bullet_Include.hpp"
#include "Common/Physics/RigidBody.hpp"

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
		GameObject* gameobject = this->parent;
		while (gameobject->parent != nullptr)
		{
			gameobject = parent->parent;

			if (gameobject->hasComponent<RigidBody>())
			{
				gameobject->getComponent<RigidBody>()->addChildShape(this, Transform(), 10.0);
				Component::enable();
				return;
			}
		}
	}
}

void CollisionShape::disable()
{
	if (this->enabled)
	{

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
