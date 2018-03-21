#include "Node.hpp"

#include "Common/Entity/EntityNode.hpp"

Node::Node()
{

}

Node::~Node()
{
	for (Node* node : this->child_nodes)
	{
		delete node;
	}

	for (auto componet : this->componentMap)
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

	for (auto componet : this->componentMap)
	{
		componet.second->update(deltaTime);
	}
}

Node * Node::getParent()
{
	return this->parent;
}

void Node::addToEntity(EntityNode* entity)
{
	this->parent_entity = entity;

	for (Node* node : this->child_nodes)
	{
		node->addToEntity(this->parent_entity);
	}

	for (auto component : this->componentMap)
	{
		component.second->addtoEntity(this->parent_entity);
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

Transform Node::getTransform()
{
	if (this->parent != nullptr)
	{
		return this->localTransform.transformBy(this->parent->getTransform());
	}
	else
	{
		return this->localTransform;
	}
}

void Node::addComponent(Component* component)
{
	ComponentType type = component->getComponentType();

	if (!this->hasComponent(type))
	{
		this->componentMap[type] = component;
		component->setParentNode(this);
		component->addtoEntity(this->parent_entity);
	}
	else
	{
		//If this node already has this component type
		printf("Error: adding duplicate component of type: %d\n", type);
		delete component;
	}
}

bool Node::hasComponent(ComponentType type)
{
	return this->componentMap.find(type) != this->componentMap.end();
}

Component* Node::getComponent(ComponentType type)
{
	if (this->hasComponent(type))
	{
		return this->componentMap.at(type);
	}

	return nullptr;
}

void Node::removeComponent(ComponentType type)
{
	if (this->hasComponent(type))
	{
		this->componentMap[type]->addtoEntity(nullptr);
		delete this->componentMap[type];
		this->componentMap.erase(this->componentMap.find(type));
	}
}

EntityNode * Node::getParentEntity()
{
	return this->parent_entity;
}

/*void Node::interactRay(Entity * entity, vector3D localStartPos, vector3D localHitPos, vector3D localHitNormal)
{
	for (auto component : this->componentMap)
	{
		component.second;
	}
}*/
