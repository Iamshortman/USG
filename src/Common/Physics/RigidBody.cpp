#include "Common/Physics/RigidBody.hpp"
#include "Common/Entity/Entity.hpp"

/*RigidBody::RigidBody(Entity* entity, double mass)
{
	this->parent = entity;

	//this->mass = mass;

	btDefaultMotionState* motionState = new btDefaultMotionState();
	btRigidBody::btRigidBodyConstructionInfo boxRigidBodyCI(mass, motionState, new btEmptyShape(), btVector3(1.0, 1.0, 1.0));
	rigidBody = new btRigidBody(boxRigidBodyCI);
}*/

RigidBody::RigidBody(Entity* entity, double mass, CollisionShape* shape)
{
	btVector3 vector = btVector3(0.0, 0.0, 0.0);

	if (mass > 0.0)
	{
		shape->btShape->calculateLocalInertia(mass, vector);
	}

	btDefaultMotionState* motionState = new btDefaultMotionState();
	btRigidBody::btRigidBodyConstructionInfo boxRigidBodyCI(mass, motionState, shape->btShape, vector);
	rigidBody = new btRigidBody(boxRigidBodyCI);
}

RigidBody::~RigidBody()
{
	delete this->rigidBody;
	//delete this->compoundShape;
}

/*int RigidBody::addChildShape(CollisionShape* shape, Transform trans)
{
	nextId++;

	this->shapes[nextId] = ChildShape();
	this->shapes[nextId].shape = shape;
	this->shapes[nextId].transform = trans;

	updateChildShapes();

	return nextId;
}

void RigidBody::removeChildShape(int i)
{
	this->shapes.erase(i);
	updateChildShapes();
}


void RigidBody::updateChildTransform(int i, Transform transform)
{
	ChildShape* child = &this->shapes[i];
	child->transform = transform;

	if (this->compoundShape != nullptr)
	{
		this->compoundShape->updateChildTransform(child->index, toBtTransform(transform), true);
	}
}

void RigidBody::updateChildShapes()
{
	btCollisionShape* oldShape = this->rigidBody->getCollisionShape();
	if (oldShape != nullptr && oldShape != this->compoundShape)
	{
		delete this->rigidBody->getCollisionShape();
	}
	else if (this->compoundShape != nullptr)
	{
		delete this->compoundShape;
	}

	if (this->shapes.size() > 0)
	{
		this->compoundShape = new btCompoundShape(true, this->shapes.size());

		//Sets Parent Index
		this->compoundShape->setUserPointer(this->parent);

		ChildShape* child;
		for (auto iter = this->shapes.begin(); iter != this->shapes.end(); iter++)
		{
			child = &iter->second;
			child->index = this->compoundShape->getNumChildShapes();

			//Sets Child Index
			child->shape->btShape->setUserIndex(child->index);
			
			this->compoundShape->addChildShape(toBtTransform(child->transform), child->shape->btShape);
		}

		this->rigidBody->setCollisionShape(this->compoundShape);
	}
	else
	{
		this->rigidBody->setCollisionShape(new btEmptyShape());
	}
}*/

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

void RigidBody::applyCentralForce(vector3D force)
{
	this->rigidBody->applyCentralForce(toBtVec3(force));
}

void RigidBody::applyCentralImpulse(vector3D impulse)
{
	this->rigidBody->applyCentralImpulse(toBtVec3(impulse));
}

void RigidBody::applyTorque(vector3D torque)
{
	this->rigidBody->applyTorque(toBtVec3(torque));
}

void RigidBody::applyTorqueImpulse(vector3D torque)
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
