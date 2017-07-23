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

	//gContactAddedCallback = collisonCallback;

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
	/*auto entities = this->parent->getEntitiesInWorld();

	for (auto it = entities->begin(); it != entities->end(); it++)
	{
		Entity* entity = *it;

		if (entity != nullptr)
		{
			RigidBody* rigidBody = entity->getRigidBody();
			if (rigidBody != nullptr)
			{
				rigidBody->setWorldTransform(entity->getTransform());
			}
		}
	}*/

	//Run Physics Simulation
	dynamicsWorld->stepSimulation(timeStep, 8, 1.0 / 120.0);

	/*for (auto it = entities->begin(); it != entities->end(); it++)
	{
		Entity* entity = *it;

		if (entity != nullptr)
		{
			RigidBody* rigidBody = entity->getRigidBody();
			if (rigidBody != nullptr)
			{
				entity->setTransform(rigidBody->getWorldTransform());
			}
		}
	}*/
}

void PhysicsWorld::addRigidBody(RigidBody* rigidBody)
{
	dynamicsWorld->addRigidBody(rigidBody->getRigidBody());
}

void PhysicsWorld::removeRigidBody(RigidBody* rigidBody)
{
	dynamicsWorld->removeRigidBody(rigidBody->getRigidBody());
}