#include "Common/Entity/EntityGridSystem.hpp"

#include "Common/World/World.hpp"
#include "Common/Physics/CollisionShapes/BoxShape.hpp"

EntityGridSystem::EntityGridSystem(EntityId id)
	:Entity(id)
{
	/*this->rigidBody = new RigidBody(this, RIGIDBODYTYPE::SINGLE);
	this->rigidBody->setMass(100.0);
	this->rigidBody->setCollisionShape(new BoxShape(vector3D(0.5)));*/

	this->rigidBody = new RigidBody(this, RIGIDBODYTYPE::COMPOUND);
	this->rigidBody->setMass(100.0);
	this->rigidBody->addChildShape(new BoxShape(vector3D(0.5, 0.25, 0.5)), Transform(vector3D(0.0, 0.25, 0.0)));
	this->rigidBody->addChildShape(new BoxShape(vector3D(0.5, 0.25, 0.5)), Transform(vector3D(0.0, -0.25, 0.0)));
}

EntityGridSystem::~EntityGridSystem()
{

}

void EntityGridSystem::update(double deltaTime)
{

}

void EntityGridSystem::interactRay(Entity* entity, vector3D localStartPos, vector3D localHitPos, vector3D localHitNormal, int bodyId)
{
	printf("EntityId: %d\n", this->entityId);
	printf("Pos: (%lf, %lf, %lf)\n", localHitPos.x, localHitPos.y, localHitPos.z);
	printf("Norm: (%lf, %lf, %lf)\n", localHitNormal.x, localHitNormal.y, localHitNormal.z);
	printf("bodyId: %d\n", bodyId);

	//this->rigidBody->Activate(true);
	//this->rigidBody->applyImpulse(glm::normalize(localHitPos - localStartPos) * 10.0 * this->rigidBody->getMass(), localHitPos);
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

