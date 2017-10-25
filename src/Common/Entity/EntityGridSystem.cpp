#include "Common/Entity/EntityGridSystem.hpp"

#include "Common/World/World.hpp"
#include "Common/Physics/CollisionShapes/BoxShape.hpp"

EntityGridSystem::EntityGridSystem(EntityId id)
	:Entity(id)
{
	this->rigidBody = new RigidBody(this, 10.0, new BoxShape(vector3D(0.5)));
}

EntityGridSystem::~EntityGridSystem()
{

}

void EntityGridSystem::update(double deltaTime)
{

}

ENTITYTYPE EntityGridSystem::getEntityType() const
{
	return ENTITYTYPE::GRIDSYSTEM;
}

void EntityGridSystem::writeNetworkPacket(BitStream* packet)
{
	Entity::writeNetworkPacket(packet);
}

void EntityGridSystem::readNetworkPacket(BitStream* packet)
{
	Entity::readNetworkPacket(packet);
}

