#include "Common/Entity/EntityNode.hpp"

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

void EntityNode::interactRay(Entity* entity, vector3D localStartPos, vector3D localHitPos, vector3D localHitNormal, int bodyId)
{
	//printf("BodyId: %d\n", bodyId);

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