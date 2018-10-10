#include "RigidBody.hpp"

#include "Common/Logger/Logger.hpp"
#include "Common/Physics/PhysicsWorld.hpp"

RigidBody::RigidBody()
{

}

RigidBody::~RigidBody()
{
	if (this->rigid_body != nullptr)
	{
		this->destroyRigidBody();
	}
}

bool RigidBody::isInWorld()
{
	return this->rigid_body != nullptr;
}

void RigidBody::createRigidBody(PhysicsWorld* world, const Transform& transform, Entity::Id id)
{
	if (this->rigid_body != nullptr)
	{
		Logger::getInstance()->logError("RigidBody already exists\n");
		return;
	}

	this->world = world;
	this->world->addRigidBody(this, transform);
	this->rigid_body->setUserData(new Entity::Id(id));

	for (auto shape : this->shapes)
	{
		this->addShapeInternal(shape.first, shape.second);
	}

	this->rigid_body->recomputeMassInformation();
}

void RigidBody::destroyRigidBody()
{
	if (this->rigid_body == nullptr)
	{
		Logger::getInstance()->logError("RigidBody doesn't exist\n");
		return;
	}

	for (auto shape : this->shapes)
	{
		this->removeShapeInternal(shape.first);
	}

	delete this->rigid_body->getUserData();
	this->world->removeRigidBody(this);

	this->rigid_body = nullptr;
	this->world = nullptr;

	for (auto shape : this->shapes)
	{
		shape.first->proxy = nullptr;
	}
}

void RigidBody::setTransform(const Transform& transform)
{
	if (this->rigid_body != nullptr)
	{
		this->rigid_body->setTransform(toRecTrans(transform));
	}
}

Transform RigidBody::getTransform()
{
	if (this->rigid_body != nullptr)
	{
		return toGlmTrans(this->rigid_body->getTransform());
	}

	return Transform();
}

void RigidBody::setLinearVelocity(const vector3D& velocity)
{
	if (this->rigid_body != nullptr)
	{
		this->rigid_body->setLinearVelocity(toRecVec3(velocity));
	}
}

vector3D RigidBody::getLinearVelocity()
{
	if (this->rigid_body != nullptr)
	{
		return toGlmVec3(this->rigid_body->getLinearVelocity());
	}

	return vector3D();
}

void RigidBody::setAngularVelocity(const vector3D& velocity)
{
	if (this->rigid_body != nullptr)
	{
		this->rigid_body->setAngularVelocity(toRecVec3(velocity));
	}
}

vector3D RigidBody::getAngularVelocity()
{
	if (this->rigid_body != nullptr)
	{
		return toGlmVec3(this->rigid_body->getAngularVelocity());
	}

	return vector3D();
}

void RigidBody::setGravityEnabled(bool enabled)
{
	if (this->rigid_body != nullptr)
	{
		this->rigid_body->enableGravity(enabled);
	}
}

bool RigidBody::getGravityEnabled()
{
	if (this->rigid_body != nullptr)
	{
		return this->rigid_body->isGravityEnabled();
	}

	return false;
}

void RigidBody::applyCenteredForce(vector3D force)
{
	if (this->rigid_body != nullptr)
	{
		this->rigid_body->applyForceToCenterOfMass(toRecVec3(force));
	}
}

void RigidBody::applyForce(vector3D world_position, vector3D force)
{
	if (this->rigid_body != nullptr)
	{
		this->rigid_body->applyForce(toRecVec3(world_position), toRecVec3(force));
	}
}

void RigidBody::applyTorque(vector3D torque)
{
	if (this->rigid_body != nullptr)
	{
		this->rigid_body->applyTorque(toRecVec3(torque));
	}
}

void RigidBody::addShape(CollisionShape* shape, Entity::Id id)
{
	this->shapes[shape] = id;

	this->addShapeInternal(shape, id);
}

void RigidBody::removeShape(CollisionShape* shape)
{
	this->shapes.erase(shape);

	this->removeShapeInternal(shape);
}

double RigidBody::getMass()
{
	if (this->rigid_body != nullptr)
	{
		return this->rigid_body->getMass();
	}

	return 0.0;
}

void RigidBody::addShapeInternal(CollisionShape* shape, Entity::Id id)
{
	if (this->rigid_body != nullptr)
	{
		shape->proxy = this->rigid_body->addCollisionShape(shape->shape, toRecTrans(shape->last_relative_transform), shape->mass);
		shape->proxy->setUserData(new Entity::Id(id));
	}
}

void RigidBody::removeShapeInternal(CollisionShape* shape)
{
	if (this->rigid_body != nullptr)
	{
		delete shape->proxy->getUserData();
		this->rigid_body->removeCollisionShape(shape->proxy);
	}
}

