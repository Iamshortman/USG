#include "SingleRigidBody.hpp"

SingleRigidBody::SingleRigidBody(Entity* entity)
: RigidBody(entity)
{
	this->empty_shape = new btEmptyShape();

	btDefaultMotionState* motionState = new btDefaultMotionState();
	btRigidBody::btRigidBodyConstructionInfo boxRigidBodyCI(this->mass, motionState, this->empty_shape, btVector3(1.0, 1.0, 1.0));
	this->rigidBody = new btRigidBody(boxRigidBodyCI);

	this->rigidBody->setUserPointer(this->parent);
}

SingleRigidBody::~SingleRigidBody()
{
	delete this->empty_shape;

	RigidBody::~RigidBody();
}

void SingleRigidBody::setShape(CollisionShape* shape)
{
	if (shape == nullptr)
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
