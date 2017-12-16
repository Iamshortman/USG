#include "Common/Entity/EntityTempShip.hpp"

#include "Common/World/World.hpp"
#include "Common/World/WorldManager.hpp"

#include "Common/Physics/CollisionShapes/ConvexMeshShape.hpp"
#include "Common/Physics/CollisionShapes/BoxShape.hpp"

EntityTempShip::EntityTempShip(EntityId id)
	:Entity(id)
{
	/*this->rigidBody = new RigidBody(this, RIGIDBODYTYPE::SINGLE);
	this->rigidBody->setMass(10000.0);
	this->rigidBody->setCollisionShape(new ConvexMeshShape("res/models/ship/outside.obj"));*/

	this->rigidBody = new RigidBody(this, RIGIDBODYTYPE::COMPOUND);
	this->rigidBody->addChildShape(new ConvexMeshShape("res/models/ship/outside.obj"), Transform(vector3D(0.0, 0.0, 0.0)));
	this->rigidBody->addChildShape(new BoxShape(vector3D(1.3, 1.3, 0.05)), Transform(vector3D(0.0, 0.0, -13.63)));
	this->rigidBody->setMass(100000.0);
	this->rigidBody->setInertiaTensor(vector3D(1.0, 1.0, 10.0) * 1000000.0);

	/*vector3D temp = this->rigidBody->getInertiaTensor();
	printf("(%lf, %lf, %lf)\n", temp.x, temp.y, temp.z);*/

	this->setSubWorld(WorldManager::instance->createWorld(WORLDTYPE::BASE));
	EntityManager::instance->createEntity(ENTITYTYPE::TEMPSHIPINSIDE)->addToWorld(this->subWorld);
	this->subWorld->setGravity(vector3D(0.0, -9.8, 0.0));
}

EntityTempShip::~EntityTempShip()
{

}

void EntityTempShip::update(double deltaTime)
{

}

void EntityTempShip::interactRay(Entity* entity, vector3D localStartPos, vector3D localHitPos, vector3D localHitNormal, int bodyId)
{
	/*printf("EntityId: %d\n", this->entityId);
	printf("Pos: (%lf, %lf, %lf)\n", localHitPos.x, localHitPos.y, localHitPos.z);
	printf("Norm: (%lf, %lf, %lf)\n", localHitNormal.x, localHitNormal.y, localHitNormal.z);
	printf("bodyId: %d\n", bodyId);*/

	if (bodyId == 1)
	{
		EntityManager::instance->ChangeWorld(entity, this->subWorld, Transform(vector3D(0.0, 0.0, -12.0)));
	}
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

