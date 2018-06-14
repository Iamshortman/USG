#include "CollisionShape.hpp"

#include "Common/Physics/Bullet_Include.hpp"
#include "Common/Physics/RigidBody.hpp"
#include "Common/Physics/PhysicsWorld.hpp"

CollisionShape::CollisionShape()
{
}

CollisionShape::~CollisionShape()
{
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
