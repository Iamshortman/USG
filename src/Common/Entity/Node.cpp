#include "Node.hpp"

#include "Common/Entity/NodeEntity.hpp"

Node::Node()
{

}

Node::~Node()
{
	for (Node* node : this->child_nodes)
	{
		delete node;
	}
}

void Node::update(double delta_time)
{
	for (Node* node : this->child_nodes)
	{
		node->update(delta_time);
	}

	I_Node::update(delta_time);
}

void Node::addToEntity(NodeEntity* entity)
{
	if (this->parent_entity != nullptr)
	{
		for (auto component : this->node_component_map)
		{
			component.second->removeFromEntity();
		}
	}

	this->parent_entity = entity;

	for (Node* node : this->child_nodes)
	{
		node->addToEntity(this->parent_entity);
	}

	if (this->parent_entity != nullptr)
	{
		for (auto component : this->node_component_map)
		{
			component.second->addToEntity();
		}
	}
}
void Node::addChild(Node* node)
{
	this->child_nodes.insert(node);
	node->parent = this;

	if (this->parent_entity != nullptr)
	{
		node->addToEntity(this->parent_entity);
	}
}

void Node::removeChild(Node* node)
{
	if (this->child_nodes.find(node) != this->child_nodes.end())
	{
		this->child_nodes.erase(node);
		node->parent = nullptr;
		node->addToEntity(nullptr);
	}
}

void Node::setLocalTransform(Transform trans)
{
	this->localTransform = trans;
}

Transform Node::getLocalTransform()
{
	return this->localTransform;
}

Transform Node::getRelativeTransform()
{
	if (this->parent != nullptr)
	{
		return this->localTransform.transformBy(this->parent->getRelativeTransform());
	}
	else
	{
		return this->localTransform;
	}
}

Transform Node::getWorldTransform()
{
	if (this->parent != nullptr)
	{
		return this->localTransform.transformBy(this->parent->getWorldTransform());
	}
	else
	{
		return this->localTransform.transformBy(this->parent_entity->getLocalTransform());
	}
}

Transform Node::getGlobalTransform()
{
	if (this->parent != nullptr)
	{
		return this->localTransform.transformBy(this->parent->getGlobalTransform());
	}
	else
	{
		if (this->parent_entity != nullptr)
		{
			return this->localTransform.transformBy(this->parent_entity->getGlobalTransform());
		}
		else
		{
			return this->localTransform;
		}
	}
}