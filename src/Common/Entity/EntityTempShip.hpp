#include "Common/Entity/Entity.hpp"

#include "Common/Physics/RigidBody.hpp"

#include "Common/Entity/EntityManager.hpp"

class EntityTempShip : public Entity
{
public:
	EntityTempShip(EntityId id);
	~EntityTempShip();
	
	bool hasSubWorld();
	World* getSubWorld();
	void setSubWorld(World* world);
	void removeSubWorld();
	RigidBody* getRigidBody();

	virtual void update(double deltaTime);
	virtual Transform getTransform();
	virtual void setTransform(Transform transform);
	virtual void addToWorld(World* world);

	virtual ENTITYTYPE getEntityType() const;

	virtual void writeNetworkPacket(BitStream* packet);
	virtual void readNetworkPacket(BitStream* packet);

protected:
	RigidBody* rigidBody = nullptr;
	World* subWorld = nullptr;
};

struct CreatorTempShip : public Creator
{
	virtual Entity* create(EntityId id)
	{
		return new EntityTempShip(id);
	};
};