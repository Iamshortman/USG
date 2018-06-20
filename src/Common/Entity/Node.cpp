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

	for (auto componet : this->component_map)
	{
		delete componet.second;
	}
}

void Node::update(double deltaTime)
{
	for (Node* node : this->child_nodes)
	{
		node->update(deltaTime);
	}

	for (auto component : this->component_map)
	{
		component.second->update(deltaTime);
	}
}

void Node::addToEntity(NodeEntity* entity)
{
	if (this->parent_entity != nullptr)
	{
		for (auto component : this->component_map)
		{
			component.second->disable();
		}
	}

	this->parent_entity = entity;

	for (Node* node : this->child_nodes)
	{
		node->addToEntity(this->parent_entity);
	}

	if (this->parent_entity != nullptr)
	{
		for (auto component : this->component_map)
		{
			component.second->enable();
		}
	}
}
void Node::addChild(Node* node)
{
	this->child_nodes.insert(node);
	node->parent = this;
	node->addToEntity(this->parent_entity);
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

Transform Node::getGlobalTransform()
{
	if (this->parent != nullptr)
	{
		return this->localTransform.transformBy(this->parent->getRelativeTransform());
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