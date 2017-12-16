#include "Common/Entity/EntityNode.hpp"

EntityNode::EntityNode(EntityId id)
	:Entity(id)
{
	this->rootNode = new Node();


	this->rootNode->child_nodes.insert(new Node());
	(*this->rootNode->child_nodes.begin())->offset = Transform(vector3D(0.0, 1.0, 0.0));
}

EntityNode::~EntityNode()
{
	delete this->rootNode;
}

void EntityNode::update(double deltaTime)
{

}

void EntityNode::interactRay(Entity* entity, vector3D localStartPos, vector3D localHitPos, vector3D localHitNormal, int bodyId)
{

}

ENTITYTYPE EntityNode::getEntityType() const
{
	return ENTITYTYPE::ENTITY_NODE;
}