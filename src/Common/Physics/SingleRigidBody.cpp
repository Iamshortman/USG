#include "SingleRigidBody.hpp"

SingleRigidBody::SingleRigidBody()
: RigidBody()
{
	this->empty_shape = new btEmptyShape();

	btDefaultMotionState* motionState = new btDefaultMotionState();
	btRigidBody::btRigidBodyConstructionInfo boxRigidBodyCI(this->mass, motionState, this->empty_shape, toBtVec3(this->inertia));
	this->rigidBody = new btRigidBody(boxRigidBodyCI);
}

SingleRigidBody::~SingleRigidBody()
{
	delete this->empty_shape;
}

void SingleRigidBody::setShape(CollisionShape* shape)
{
	if (shape == nullptr || shape->getShape() == nullptr)
	{
		this->shape = nullptr;
		this->setCollisionShape(this->empty_shape);
	}
	else
	{
		this->shape = shape;
		this->setCollisionShape(shape->getShape());
	}
}

CollisionShape* SingleRigidBody::getShape()
{
	return this->shape;
}
