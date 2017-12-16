#ifndef ENTITYTEMPSHIPINSIDE_HPP
#define ENTITYTEMPSHIPINSIDE_HPP

#include "Common/Entity/Entity.hpp"

#include "Common/Physics/RigidBody.hpp"
#include "Common/Entity/EntityTempShip.hpp" 

class EntityTempShipInside : public Entity
{
public:
	EntityTempShipInside(EntityId id);
	~EntityTempShipInside();

	virtual void update(double deltaTime);

	//virtual void interactRay(Entity* entity, vector3D localStartPos, vector3D localHitPos, vector3D localHitNormal, int bodyId = 0);

	virtual ENTITYTYPE getEntityType() const;

	virtual void writeNetworkPacket(BitStream* packet);
	virtual void readNetworkPacket(BitStream* packet);

protected:

};

struct CreatorTempShipInside : public Creator
{
	virtual Entity* create(EntityId id)
	{
		return new EntityTempShipInside(id);
	};
};

#endif //ENTITYTEMPSHIPINSIDE_HPP