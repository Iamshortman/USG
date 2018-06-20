#include "MuiltiRigidBody.hpp"

#include "Common/Entity/Node.hpp"
#include "Common/Physics/PhysicsWorld.hpp"

MuiltiRigidBody::MuiltiRigidBody(Entity* entity)
: RigidBody(entity)
{
	this->empty_shape = new btEmptyShape();

	this->compound_shape = new btCompoundShape();

	btDefaultMotionState* motionState = new btDefaultMotionState();
	btRigidBody::btRigidBodyConstructionInfo boxRigidBodyCI(this->mass, motionState, this->empty_shape, toBtVec3(this->inertia));
	this->rigidBody = new btRigidBody(boxRigidBodyCI);

	this->rigidBody->setUserPointer(this->parent);
}

MuiltiRigidBody::~MuiltiRigidBody()
{
	delete this->empty_shape;
	delete this->compound_shape;

	RigidBody::~RigidBody();
}

void MuiltiRigidBody::addChildShape(Node* node)
{
	if (node->hasComponent<CollisionShape>())
	{
		if (this->rigidBody->getCollisionShape() == this->empty_shape)
		{
			this->setCollisionShape(this->compound_shape);
		}

		CollisionShape* shape = node->getComponent<CollisionShape>();

		this->compound_shape->addChildShape(toBtTransform(node->getRelativeTransform()), shape->getShape());
	}
}

void MuiltiRigidBody::removeChildShape(Node* node)
{
	if (node->hasComponent<CollisionShape>())
	{
		CollisionShape* shape = node->getComponent<CollisionShape>();

		this->compound_shape->removeChildShape(shape->getShape());

		if (this->compound_shape->getNumChildShapes() <= 0)
		{
			this->setCollisionShape(this->empty_shape);
		}
	}
}

void MuiltiRigidBody::updateChildShapeTransform(Node* node)
{
	if (node->hasComponent<CollisionShape>())
	{
		CollisionShape* shape = node->getComponent<CollisionShape>();

		this->compound_shape->removeChildShape(shape->getShape());
		this->compound_shape->addChildShape(toBtTransform(node->getRelativeTransform()), shape->getShape());
	}
}
