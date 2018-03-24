#ifndef ENTITYNODE_HPP
#define ENTITYNODE_HPP

#include "Common/Entity/Entity.hpp"
#include "Common/Entity/EntityManager.hpp"

#include <set>
#include "Node.hpp"

//prototype class
class ComponentModel;
class ComponentLight;

class EntityNode: public Entity
{
public:
	EntityNode(EntityId id);
	~EntityNode();

	virtual void update(double deltaTime);
	virtual void addToWorld(World* world);

	virtual void interactRay(Entity* entity, vector3D localStartPos, vector3D localHitPos, vector3D localHitNormal, int bodyId = -1);

	virtual ENTITYTYPE getEntityType() const;

	//virtual void writeNetworkPacket(BitStream* packet);
	//virtual void readNetworkPacket(BitStream* packet);

	Node* rootNode = nullptr;

	void addModel(ComponentModel* model);
	void removeModel(ComponentModel* model);

	void addLight(ComponentLight* light);
	void removeLight(ComponentLight* light);

	std::set<ComponentModel*> models;
	std::set<ComponentLight*> lights;

protected:
};

struct CreatorEntityNode : public Creator
{
	virtual Entity* create(EntityId id)
	{
		return new EntityNode(id);
	};
};

#endif //ENTITYNODE_HPP