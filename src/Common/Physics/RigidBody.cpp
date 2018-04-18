#include "Common/Physics/RigidBody.hpp"

#include "Common/GameObject.hpp"
#include "Common/Physics/PhysicsWorld.hpp"

RigidBody::RigidBody()
{
	emptyShape = new btEmptyShape();

	btDefaultMotionState* motionState = new btDefaultMotionState();
	btRigidBody::btRigidBodyConstructionInfo boxRigidBodyCI(mass, motionState, emptyShape, btVector3(0.0, 0.0, 0.0));
	this->rigidBody = new btRigidBody(boxRigidBodyCI);
}

RigidBody::~RigidBody()
{
	this->disable();

	if (this->rigidBody != nullptr)
	{
		delete this->rigidBody;
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

void RigidBody::enable()
{
	if (!this->enabled)
	{
		this->rigidBody->setWorldTransform(toBtTransform(this->parent->getLocalTransform()));

		if (this->parent->parent != nullptr)
		{
			if (this->parent->parent->hasComponent<PhysicsWorld>())
			{
				this->world = parent->parent->getComponent<PhysicsWorld>();
				this->world->addRigidBody(this);
				this->rigidBody->setUserPointer(this->parent);
				Component::enable();
			}
			else
			{
				printf("Error: No World\n");
			}
		}
		else
		{
			printf("Error: No Parent\n");
		}

		Component::enable();
	}
}

void RigidBody::disable()
{
	if (this->enabled)
	{
		if (this->world != nullptr)
		{
			this->world->removeRigidBody(this);
			this->world = nullptr;
		}
		Component::disable();
	}
}

childId RigidBody::addChildShape(CollisionShape* shape, Transform transform, double mass, GameObject* gameObject)
{
	childId id = getNextId();

	this->childShapes[id] = ChildShape();
	this->childShapes[id].shape = shape;
	this->childShapes[id].transform = transform;
	this->childShapes[id].gameObject = gameObject;
	this->childShapes[id].mass = mass;

	this->rebuildCompondShape();

	return id;
}

GameObject* RigidBody::getChildNode(childId id)
{
	if (this->childShapes.find(id) != this->childShapes.end())
	{
		return this->childShapes[id].gameObject;
	}

	return nullptr;
}

void RigidBody::removeChildShape(childId id)
{
	this->childShapes.erase(id);
	this->rebuildCompondShape();
}


void RigidBody::updateChildTransform(childId id, Transform transform)
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

void RigidBody::rebuildCompondShape()
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
		//this->compoundShape->setUserPointer(this->parent);

		double totalMass = 0.01;

		ChildShape* child;
		for (auto iter = this->childShapes.begin(); iter != this->childShapes.end(); iter++)
		{
			child = &iter->second;
			child->index = this->compoundShape->getNumChildShapes();

			//Sets Child Index
			child->shape->btShape->setUserIndex(*&iter->first);
			//child->shape->btShape->setUserPointer(this->parent);

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