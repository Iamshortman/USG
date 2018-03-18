#include "Common/Physics/RigidBody.hpp"
#include "Common/Entity/Entity.hpp"
#include "Common/Entity/Node.hpp"

RigidBody::RigidBody(Entity* entity, RIGIDBODYTYPE type)
{
	emptyShape = new btEmptyShape();

	btDefaultMotionState* motionState = new btDefaultMotionState();
	btRigidBody::btRigidBodyConstructionInfo boxRigidBodyCI(mass, motionState, emptyShape, btVector3(0.0, 0.0, 0.0));
	this->rigidBody = new btRigidBody(boxRigidBodyCI);

	this->parent = entity;
	this->rigidBody->setUserPointer(entity);

	this->type = type;
}

RigidBody::~RigidBody()
{
	if (this->rigidBody != nullptr)
	{
		delete this->rigidBody;
	}

	if (this->singleShape != nullptr)
	{
		delete this->singleShape;
	}

	if (this->compoundShape != nullptr)
	{
		delete this->compoundShape;
	}

	for (auto it = this->childShapes.begin(); it != this->childShapes.end(); it++)
	{
		delete it->second.shape;
	}

	if (this->emptyShape != nullptr)
	{
		delete this->emptyShape;
	}
}

void RigidBody::setCollisionShape(CollisionShape* shape)
{
	if (this->type == RIGIDBODYTYPE::SINGLE)
	{
		if (this->singleShape != nullptr)
		{
			delete this->singleShape;
		}

		this->singleShape = shape;

		if (this->singleShape != nullptr)
		{
			this->singleShape->btShape->setUserIndex(-1);
			this->singleShape->btShape->setUserPointer(this->parent);
			this->rigidBody->setCollisionShape(this->singleShape->btShape);
			this->setMass(this->mass);
			this->setInertiaTensor(vector3D(0.0));
		}
		else
		{
			this->rigidBody->setCollisionShape(this->emptyShape);
		}
	}
}

CollisionShape* RigidBody::getCollisionShape()
{
	return this->singleShape;
}

childId RigidBody::addChildShape(CollisionShape* shape, Transform transform, double mass, Node* node)
{
	if (this->type == RIGIDBODYTYPE::COMPOUND)
	{
		childId id = getNextId();

		this->childShapes[id] = ChildShape();
		this->childShapes[id].shape = shape;
		this->childShapes[id].transform = transform;
		this->childShapes[id].node = node;
		this->childShapes[id].mass = mass;

		this->rebuildCompondShape();

		return id;
	}

	return -1;
}

Node * RigidBody::getChildNode(childId id)
{
	if (this->type == RIGIDBODYTYPE::COMPOUND)
	{
		if (this->childShapes.find(id) != this->childShapes.end())
		{
			return this->childShapes[id].node;
		}
	}

	return nullptr;
}

void RigidBody::removeChildShape(childId id)
{
	if (this->type == RIGIDBODYTYPE::COMPOUND)
	{
		this->childShapes.erase(id);
		this->rebuildCompondShape();
	}
}


void RigidBody::updateChildTransform(childId id, Transform transform)
{
	if (this->type == RIGIDBODYTYPE::COMPOUND)
	{
		if (this->childShapes.find(id) != this->childShapes.end())
		{
			ChildShape* child = &this->childShapes[id];
			child->transform = transform;

			if (this->compoundShape != nullptr)
			{
				this->compoundShape->updateChildTransform(child->index, toBtTransform(transform), true);
			}
		}
	}
}

void RigidBody::rebuildCompondShape()
{
	if (this->type == RIGIDBODYTYPE::COMPOUND)
	{
		btCollisionShape* oldShape = this->rigidBody->getCollisionShape();

		if (oldShape != nullptr && oldShape != this->emptyShape)
		{
			delete this->rigidBody->getCollisionShape();

			if (oldShape == this->compoundShape)
			{
				this->compoundShape = nullptr;
			}
		}

		if (this->childShapes.size() > 0)
		{
			this->compoundShape = new btCompoundShape(true, (int) this->childShapes.size());

			//Sets Parent Index
			this->compoundShape->setUserPointer(this->parent);

			double totalMass = 0.01;

			ChildShape* child;
			for (auto iter = this->childShapes.begin(); iter != this->childShapes.end(); iter++)
			{
				child = &iter->second;
				child->index = this->compoundShape->getNumChildShapes();

				//Sets Child Index
				child->shape->btShape->setUserIndex(*&iter->first);
				child->shape->btShape->setUserPointer(this->parent);

				this->compoundShape->addChildShape(toBtTransform(child->transform), child->shape->btShape);

				totalMass += child->mass;
			}

			if (this->mass == 0.0)
			{
				totalMass = 0.0;
			}

			this->mass = totalMass;

			this->rigidBody->setMassProps(this->mass, toBtVec3(this->inertia));

			this->rigidBody->setCollisionShape(this->compoundShape);
		}
		else
		{
			this->rigidBody->setCollisionShape(this->emptyShape);
		}
	}
}

void RigidBody::setMass(double massToAdd)
{
	this->mass = massToAdd;
	this->rigidBody->setMassProps(this->mass, toBtVec3(this->inertia));
}

double RigidBody::getMass()
{
	return this->mass;
}

void RigidBody::setInertiaTensor(vector3D inertiaToSet)
{
	this->inertia = inertiaToSet;
	this->rigidBody->setMassProps(this->mass, toBtVec3(this->inertia));
}

vector3D RigidBody::getInertiaTensor()
{
	return this->inertia;
}

void RigidBody::Activate(bool activate)
{
	this->rigidBody->activate(activate);
}

Transform RigidBody::getWorldTransform()
{
	return toTransform(this->rigidBody->getWorldTransform());
}

void RigidBody::setWorldTransform(Transform transform)
{
	this->rigidBody->setCenterOfMassTransform(toBtTransform(transform));
}

vector3D RigidBody::getLinearVelocity() const
{
	return toVec3(this->rigidBody->getLinearVelocity());
}

void RigidBody::setLinearVelocity(vector3D velocity)
{
	this->rigidBody->setLinearVelocity(toBtVec3(velocity));
}

vector3D RigidBody::getAngularVelocity() const
{
	return toVec3(this->rigidBody->getAngularVelocity());
}

void RigidBody::setAngularVelocity(vector3D velocity)
{
	this->rigidBody->setAngularVelocity(toBtVec3(velocity));
}

void RigidBody::applyForce(vector3D &force, vector3D &localPos)
{
	this->rigidBody->applyForce(toBtVec3(force), toBtVec3(localPos));
}

void RigidBody::applyImpulse(vector3D &impulse, vector3D &localPos)
{
	this->rigidBody->applyImpulse(toBtVec3(impulse), toBtVec3(localPos));
}

void RigidBody::applyCentralForce(vector3D &force)
{
	this->rigidBody->applyCentralForce(toBtVec3(force));
}

void RigidBody::applyCentralImpulse(vector3D &impulse)
{
	this->rigidBody->applyCentralImpulse(toBtVec3(impulse));
}

void RigidBody::applyTorque(vector3D &torque)
{
	this->rigidBody->applyTorque(toBtVec3(torque));
}

void RigidBody::applyTorqueImpulse(vector3D &torque)
{
	this->rigidBody->applyTorqueImpulse(toBtVec3(torque));
}

void RigidBody::setDampening(double linear, double angular)
{
	rigidBody->setDamping(linear, angular);
}

btRigidBody* RigidBody::getRigidBody()
{
	return this->rigidBody;
}

childId RigidBody::getNextId()
{
	childId id = 0;

	while (this->childShapes.find(id) != this->childShapes.end())
	{
		id++;
	}

	return id;
}