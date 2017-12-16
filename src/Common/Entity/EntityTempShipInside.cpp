#include "Common/Entity/EntityTempShipInside.hpp"

#include "Common/World/World.hpp"
#include "Common/Physics/CollisionShapes/ConcaveMeshShape.hpp"
#include "Common/Physics/CollisionShapes/BoxShape.hpp"

EntityTempShipInside::EntityTempShipInside(EntityId id)
	:Entity(id)
{
	this->rigidBody = new RigidBody(this, RIGIDBODYTYPE::SINGLE);
	this->rigidBody->setMass(0.0);
	this->rigidBody->setCollisionShape(new ConcaveMeshShape("res/models/ship/inside.obj"));
}

EntityTempShipInside::~EntityTempShipInside()
{

}

void EntityTempShipInside::update(double deltaTime)
{

}

ENTITYTYPE EntityTempShipInside::getEntityType() const
{
	return ENTITYTYPE::TEMPSHIPINSIDE;
}

void EntityTempShipInside::writeNetworkPacket(BitStream* packet)
{
	Entity::writeNetworkPacket(packet);
}

void EntityTempShipInside::readNetworkPacket(BitStream* packet)
{
	Entity::readNetworkPacket(packet);
}

