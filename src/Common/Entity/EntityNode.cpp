#include "Common/Entity/EntityNode.hpp"
#include "Common/World/World.hpp"

EntityNode::EntityNode(EntityId id)
	:Entity(id)
{
	this->rigidBody = new RigidBody(this, RIGIDBODYTYPE::COMPOUND);

	this->rootNode = new Node();
	this->rootNode->addToEntity(this);
}

EntityNode::~EntityNode()
{
	delete this->rootNode;
}

void EntityNode::update(double deltaTime)
{
	if (this->rootNode != nullptr)
	{
		this->rootNode->update(deltaTime);
	}
}

void EntityNode::addToWorld(World* world)
{
	Entity::addToWorld(world);
}

void EntityNode::interactRay(Entity* entity, vector3D localStartPos, vector3D localHitPos, vector3D localHitNormal, int bodyId)
{
	Node* node = this->rigidBody->getChildNode(bodyId);
	if (node != nullptr)
	{
		if (node->getParent() != nullptr)
		{
			Transform temp = node->getTransform().transformBy(this->getTransform());
			node->getParent()->removeChild(node);

			EntityNode* newNode = (EntityNode*)EntityManager::instance->createEntity(ENTITYTYPE::ENTITY_NODE);

			delete newNode->rootNode;
			newNode->rootNode = node;
			node->setLocalTransform(Transform());
			newNode->rootNode->addToEntity(newNode);

			newNode->setTransform(temp);
			newNode->addToWorld(this->world);
		}
	}
}

ENTITYTYPE EntityNode::getEntityType() const
{
	return ENTITYTYPE::ENTITY_NODE;
}

void EntityNode::addModel(ComponentModel* model)
{
	if (this->models.find(model) == this->models.end())
	{
		this->models.insert(model);
	}
}

void EntityNode::removeModel(ComponentModel* model)
{
	if (this->models.find(model) != this->models.end())
	{
		this->models.erase(model);
	}
}
