#include "Common/Entity/Entity.hpp"

#include "Common/Physics/RigidBody.hpp"

#include "Common/Entity/EntityManager.hpp"

class EntityGridSystem : public Entity
{
public:
	EntityGridSystem(EntityId id);
	~EntityGridSystem();

	virtual void update(double deltaTime);

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