#include "Common/Entity/ComponentCollisionShape.hpp"
#include "Common/Entity/EntityNode.hpp"

ComponentCollisionShape::ComponentCollisionShape(CollisionShape* shape, double mass)
{
	this->shape = shape;
	this->mass = mass;
}

ComponentCollisionShape::~ComponentCollisionShape()
{
	this->disable();

	delete this->shape;
}

void ComponentCollisionShape::update(double deltaTime)
{
	if (this->enabled)
	{
		Transform temp = this->parent->getTransform();
		if (temp != this->current_transform)
		{
			this->current_transform = temp;
			this->parent_entity->getRigidBody()->updateChildTransform(this->childShapeId, this->current_transform);
		}
	}
}

void ComponentCollisionShape::enable()
{
	if (!this->enabled && this->parent_entity != nullptr)
	{
		RigidBody* rigidBody = this->parent_entity->getRigidBody();
		if (rigidBody == nullptr)
		{
			printf("Error: No RigidBody\n");
			return;
		}

		this->current_transform = this->parent->getTransform();
		this->childShapeId = rigidBody->addChildShape(this->shape, this->current_transform, this->mass, this->parent);

		Component::enable();
	}
}

void ComponentCollisionShape::disable()
{
	if (this->enabled && this->parent_entity != nullptr)
	{
		Component::disable();

		RigidBody* rigidBody = this->parent_entity->getRigidBody();
		if (rigidBody == nullptr)
		{
			printf("Error: No RigidBody\n");
			return;
		}

		this->current_transform = Transform();
		rigidBody->removeChildShape(this->childShapeId);
	}
}

void ComponentCollisionShape::addtoEntity(EntityNode* entity)
{
	this->disable();

	Component::addtoEntity(entity);

	this->enable();
}

ComponentType ComponentCollisionShape::getComponentType() const
{
	return ComponentType::COLLISIONSHAPE;
}
