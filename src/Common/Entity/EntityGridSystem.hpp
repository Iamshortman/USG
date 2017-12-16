#include "Common/Entity/Entity.hpp"
#include "Common/Entity/EntityManager.hpp"

class EntityGridSystem : public Entity
{
public:
	EntityGridSystem(EntityId id);
	~EntityGridSystem();

	virtual void update(double deltaTime);

	virtual void interactRay(Entity* entity, vector3D localStartPos, vector3D localHitPos, vector3D localHitNormal, int bodyId = -1);

	virtual ENTITYTYPE getEntityType() const;

	virtual void writeNetworkPacket(BitStream* packet);
	virtual void readNetworkPacket(BitStream* packet);

protected:

};

struct CreatorGridSystem : public Creator
{
	virtual Entity* create(EntityId id)
	{
		return new EntityGridSystem(id);
	};
};