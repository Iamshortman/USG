#include "Common/Entity/EntityTempShip.hpp"

#include "Common/World/World.hpp"
#include "Common/Physics/CollisionShapes/BoxShape.hpp"

EntityTempShip::EntityTempShip(EntityId id)
	:Entity(id)
{
	this->rigidBody = new RigidBody(this, 100.0, new BoxShape(vector3D(1.5, 1.5, 4.5)));
}

EntityTempShip::~EntityTempShip()
{

}

void EntityTempShip::update(double deltaTime)
{

}

ENTITYTYPE EntityTempShip::getEntityType() const
{
	return ENTITYTYPE::TEMPSHIP;
}

void EntityTempShip::writeNetworkPacket(BitStream* packet)
{
	Entity::writeNetworkPacket(packet);
}

void EntityTempShip::readNetworkPacket(BitStream* packet)
{
	Entity::readNetworkPacket(packet);
}

