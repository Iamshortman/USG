#include "Common/Physics/PhysicsWorld.hpp"
#include "Common/World/World.hpp"

PhysicsWorld::PhysicsWorld(World* world)
{
	this->parent = world;

	// Build the broadphase
	broadphase = new btDbvtBroadphase();

	// Set up the collision configuration and dispatcher
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	// The actual physics solver
	solver = new btSequentialImpulseConstraintSolver();

	// The world
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0.0, 0.0, 0.0));
}

PhysicsWorld::~PhysicsWorld()
{
	// Clean up behind ourselves like good little programmers
	delete dynamicsWorld;
	delete solver;
	delete dispatcher;
	delete collisionConfiguration;
	delete broadphase;
}

void PhysicsWorld::update(double timeStep)
{
	//Run Physics Simulation
	this->dynamicsWorld->stepSimulation(timeStep, 8, 1.0 / 120.0);


	int numManifolds = this->dynamicsWorld->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* contactManifold = this->dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject* obA = contactManifold->getBody0();
		const btCollisionObject* obB = contactManifold->getBody1();

		Entity* ptr1 = (Entity*)obA->getUserPointer();
		Entity* ptr2 = (Entity*)obB->getUserPointer();

		/*if (ptr->getEntityType() == ENTITYTYPE::GRIDSYSTEM)
		{
			printf("Hit: %d\n", ptr->entityId);
		}*/

		/*int numContacts = contactManifold->getNumContacts();
		for (int j = 0; j < numContacts; j++)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			if (pt.getDistance() < 0.f)
			{
				const btVector3& ptA = pt.getPositionWorldOnA();
				const btVector3& ptB = pt.getPositionWorldOnB();
				const btVector3& normalOnB = pt.m_normalWorldOnB;
			}
		}*/
	}
}

void PhysicsWorld::addRigidBody(RigidBody* rigidBody)
{
	this->dynamicsWorld->addRigidBody(rigidBody->getRigidBody());
}

void PhysicsWorld::removeRigidBody(RigidBody* rigidBody)
{
	this->dynamicsWorld->removeRigidBody(rigidBody->getRigidBody());
}