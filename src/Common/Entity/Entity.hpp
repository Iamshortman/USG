#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>
#include <vector>

#include "Common/GLM_Include.hpp"
#include "Common/Transform.hpp"
#include "Common/Networking/Packet.hpp"
#include "Common/Physics/RigidBody.hpp"

enum ENTITYTYPE
{
	GRIDSYSTEM,
	PLAYER_OTHER,
	PLAYER_THIS,
	CHARACTOR,
	TEMPSHIP,
};

//Prototype Classe
class World;

typedef uint32_t EntityId;

class Entity
{
public:
	const EntityId entityId;

	Entity(EntityId id);
	virtual ~Entity();

	bool isAlive() { return alive; };
	void kill();
	
	virtual void update(double deltaTime) = 0;
	virtual void addToWorld(World* world);
	virtual World* getWorld();
	virtual Transform getTransform();
	virtual void setTransform(Transform transform);
	Transform getRenderTransform();

	bool hasSubWorld();
	World* getSubWorld();
	void setSubWorld(World* world);
	void removeSubWorld();
	RigidBody* getRigidBody();

	virtual ENTITYTYPE getEntityType() const = 0;

	virtual void writeNetworkPacket(BitStream* packet);
	virtual void readNetworkPacket(BitStream* packet);

protected:
	World* world = nullptr;
	bool alive = true;
	Transform transform;

	RigidBody* rigidBody = nullptr;
	World* subWorld = nullptr;
};

#endif //ENTITY_HPP