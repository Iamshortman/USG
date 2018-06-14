#include "Common/Physics/RigidBody.hpp"

#include "Common/Physics/PhysicsWorld.hpp"

#include "Common/Logger/Logger.hpp"

#define DEFAULT_MASS 100.0

RigidBody::RigidBody(bool is_static)
	:is_static(is_static)
{
	this->emptyShape = new btEmptyShape();

	//btCollisionShape* shape = new btBoxShape(btVector3(0.5, 0.5, 0.5));
}

RigidBody::~RigidBody()
{

	if (this->compoundShape != nullptr)
	{
		delete this->compoundShape;
	}

	if (this->emptyShape != nullptr)
	{
		delete this->emptyShape;
	}
}

int RigidBody::addChildShape(CollisionShape* shape)
{
	int id = getNextId();

	this->childShapes[id] = shape;

	return id;
}

void RigidBody::removeChildShape(int id)
{
	if (id < 0)
	{
		return;
	}

	this->childShapes.erase(id);
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

int RigidBody::getNextId()
{
	int id = 0;

	while (this->childShapes.find(id) != this->childShapes.end())
	{
		id++;
	}

	return id;
}