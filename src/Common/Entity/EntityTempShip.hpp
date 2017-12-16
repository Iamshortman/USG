#ifndef ENTITYTEMPSHIP_HPP
#define ENTITYTEMPSHIP_HPP

#include "Common/Entity/Entity.hpp"

#include "Common/Physics/RigidBody.hpp"

#include "Common/Entity/EntityManager.hpp"

class EntityTempShip : public Entity
{
public:
	EntityTempShip(EntityId id);
	~EntityTempShip();

	virtual void update(double deltaTime);

	virtual void interactRay(Entity* entity, vector3D localStartPos, vector3D localHitPos, vector3D localHitNormal, int bodyId = 0);

	virtual ENTITYTYPE getEntityType() const;

	virtual void writeNetworkPacket(BitStream* packet);
	virtual void readNetworkPacket(BitStream* packet);

protected:

};

struct CreatorTempShip : public Creator
{
	virtual Entity* create(EntityId id)
	{
		return new EntityTempShip(id);
	};
};

#endif //ENTITYTEMPSHIP_HPP