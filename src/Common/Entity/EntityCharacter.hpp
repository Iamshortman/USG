#ifndef ENTITYCHARACTER_HPP
#define ENTITYCHARACTER_HPP

#include "Common/Entity/Entity.hpp"

#include "Common/Physics/RigidBody.hpp"
#include "Common/Entity/EntityManager.hpp"

class EntityCharacter: public Entity
{
public:
	EntityCharacter(EntityId id);
	virtual ~EntityCharacter();

	virtual void update(double deltaTime);

	Transform getEyeTransform();

	virtual ENTITYTYPE getEntityType() const;

	virtual void writeNetworkPacket(BitStream* packet);
	virtual void readNetworkPacket(BitStream* packet);

protected:
	bool inGravity = false;

	double groundSpeed = 5.0;
	double groundTurnSpeed = 0.75f;

	double headPitch = 0.0;
	//double headYaw = 0.0;

	double spaceMaxSpeed = 50.0;
	double spaceAcceleration = 0.5;

	double spaceTurnSpeed = 0.25;

	vector3D linearInput = vector3D(0.0);
	vector3D angularInput = vector3D(0.0);
	bool interact = false;
};

struct CreatorCharacter : public Creator
{
	virtual Entity* create(EntityId id)
	{
		return new EntityCharacter(id);
	};
};

#endif //ENTITYCHARACTER_HPP