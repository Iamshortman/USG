#ifndef ENTITYPLAYERCLIENT_HPP
#define ENTITYPLAYERCLIENT_HPP

#include "Common/Entity/EntityCharacter.hpp"
#include "Client/Entity/PlayerControllerInterface.hpp"

#include "Common/Entity/EntityManager.hpp"

class EntityPlayerClient : public EntityCharacter, PlayerControllerInterface
{
public:
	EntityPlayerClient(EntityId id);
	virtual ~EntityPlayerClient();

	virtual void update(double deltaTime);

	virtual void updatePlayerInput();
	virtual Transform getCameraTransform();

	virtual void writeNetworkPacket(BitStream* packet);
	virtual void readNetworkPacket(BitStream* packet);

	virtual ENTITYTYPE getEntityType() const;
};

struct CreatorPlayerClient : public Creator
{
	virtual Entity* create(EntityId id)
	{
		return new EntityPlayerClient(id);
	};
};

#endif //ENTITYPLAYERCLIENT_HPP