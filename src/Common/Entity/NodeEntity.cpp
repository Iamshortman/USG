#include "NodeEntity.hpp"

#include "Common/Physics/MuiltiRigidBody.hpp"
#include "Common/World/World.hpp"
#include "Common/Logger/Logger.hpp"

NodeEntity::NodeEntity(EntityId objectId)
:Entity(objectId)
{

}


NodeEntity::~NodeEntity()
{

}

void NodeEntity::update(double delta_time)
{
	Entity::update(delta_time);

	for (auto child : this->child_nodes)
	{
		child->update(delta_time);
	}
}

void NodeEntity::addRigidBody()
{
	if (this->rigidBody == nullptr)
	{
		this->rigidBody = new MuiltiRigidBody(this);
		this->rigidBody->setWorldTransform(this->local_transform);

		if (this->world != nullptr)
		{
			this->world->addRigidBody(this->rigidBody);
		}
	}
}

void NodeEntity::removeRigidBody()
{
	if (this->rigidBody != nullptr)
	{
		if (this->world != nullptr)
		{
			this->world->removeRigidBody(this->rigidBody);
		}

		delete this->rigidBody;
		this->rigidBody = nullptr;
	}
}

void NodeEntity::addChild(Node* node)
{
	this->child_nodes.insert(node);
	node->parent = nullptr;
	node->addToEntity(this);
}

void NodeEntity::removeChild(Node * node)
{
	if (this->child_nodes.find(node) != this->child_nodes.end())
	{
		this->child_nodes.erase(node);
		node->parent = nullptr;
		node->addToEntity(nullptr);
	}
}
