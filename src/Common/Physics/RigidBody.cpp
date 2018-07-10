#include "Common/Physics/RigidBody.hpp"

#include "Common/Physics/PhysicsWorld.hpp"

#include "Common/Logger/Logger.hpp"

RigidBody::RigidBody()
{
}

RigidBody::~RigidBody()
{
	if (this->physics_world != nullptr)
	{
		this->physics_world->removeRigidBody(this);
		this->physics_world = nullptr;
	}

	if (this->rigidBody != nullptr)
	{
		delete this->rigidBody;
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

void RigidBody::setInertiaTensor(vector3D &inertiaToSet)
{
	this->inertia = inertiaToSet;
	this->rigidBody->setMassProps(this->mass, toBtVec3(this->inertia));
}

vector3D RigidBody::getInertiaTensor()
{
	return this->inertia;
}

void RigidBody::calcInertiaTensorFromShape()
{
	if (this->mass > 0.0)
	{
		btVector3 inertia_tensor = btVector3(0.0, 0.0, 0.0);
		this->rigidBody->getCollisionShape()->calculateLocalInertia(this->mass, inertia_tensor);
		this->inertia = toVec3(inertia_tensor);
	}
}

void RigidBody::Activate(bool activate)
{
	this->rigidBody->activate(activate);
}

Transform RigidBody::getWorldTransform()
{
	return toTransform(this->rigidBody->getWorldTransform());
}

void RigidBody::setWorldTransform(Transform &transform)
{
	this->rigidBody->setCenterOfMassTransform(toBtTransform(transform));
}

vector3D RigidBody::getLinearVelocity() const
{
	return toVec3(this->rigidBody->getLinearVelocity());
}

void RigidBody::setLinearVelocity(vector3D &velocity)
{
	this->rigidBody->setLinearVelocity(toBtVec3(velocity));
}

vector3D RigidBody::getAngularVelocity() const
{
	return toVec3(this->rigidBody->getAngularVelocity());
}

void RigidBody::setAngularVelocity(vector3D &velocity)
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

void RigidBody::setCollisionShape(btCollisionShape* shape)
{
	if (this->physics_world != nullptr)
	{
		PhysicsWorld* world = this->physics_world;
		this->physics_world->removeRigidBody(this);
		this->rigidBody->setCollisionShape(shape);
		world->addRigidBody(this);
	}
	else
	{
		this->rigidBody->setCollisionShape(shape);
	}
}
