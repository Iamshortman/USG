#include "Single_RigidBody.hpp"

Single_RigidBody::Single_RigidBody(Entity* entity)
: RigidBody(entity)
{
	this->empty_shape = new btEmptyShape();

	btDefaultMotionState* motionState = new btDefaultMotionState();
	btRigidBody::btRigidBodyConstructionInfo boxRigidBodyCI(this->mass, motionState, this->empty_shape, toBtVec3(this->inertia));
	this->rigidBody = new btRigidBody(boxRigidBodyCI);

	this->rigidBody->setUserPointer(this->parent);
}

Single_RigidBody::~Single_RigidBody()
{
	delete this->empty_shape;

	RigidBody::~RigidBody();
}

void Single_RigidBody::setShape(CollisionShape* shape)
{
	if (shape == nullptr)
	{
		this->shape = nullptr;
		this->rigidBody->setCollisionShape(this->empty_shape);
	}
	else
	{
		this->shape = shape;
		this->rigidBody->setCollisionShape(shape->getShape());
	}
}

CollisionShape* Single_RigidBody::getShape()
{
	return this->shape;
}
