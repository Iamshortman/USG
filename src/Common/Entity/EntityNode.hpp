#include "Common/Entity/Entity.hpp"
#include "Common/Entity/EntityManager.hpp"

#include <set>

class leaf;

class Node
{
public:
	Node* parent;
	std::set<Node*> child_nodes;
	std::set<leaf*> child_leaves;
	Transform offset;

	Transform getTransform()
	{
		if (this->parent != nullptr)
		{
			return this->offset.transformBy(this->parent->getTransform());
		}
		else
		{
			return this->offset;
		}
	};
};


class EntityNode: public Entity
{
public:
	EntityNode(EntityId id);
	~EntityNode();

	virtual void update(double deltaTime);

	virtual void interactRay(Entity* entity, vector3D localStartPos, vector3D localHitPos, vector3D localHitNormal, int bodyId = -1);

	virtual ENTITYTYPE getEntityType() const;

	//virtual void writeNetworkPacket(BitStream* packet);
	//virtual void readNetworkPacket(BitStream* packet);

	Node* rootNode = nullptr;

protected:
};

struct CreatorEntityNode : public Creator
{
	virtual Entity* create(EntityId id)
	{
		return new EntityNode(id);
	};
};