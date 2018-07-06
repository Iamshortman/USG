#ifndef PHYSICSWORLD_HPP
#define PHYSICSWORLD_HPP

#include "Common/Physics/Bullet_Include.hpp"
#include "Common/Physics/RigidBody.hpp"

#include <set>

struct SingleRayTestResult
{
	bool hasHit = false;

	vector3D hitPosition;
	vector3D hitNormal;
};

class PhysicsWorld
{

public:
	PhysicsWorld();
	virtual ~PhysicsWorld();

	virtual void update(double delta_time);

	void addRigidBody(RigidBody* rigidBody);
	void removeRigidBody(RigidBody* rigidBody);

	SingleRayTestResult singleRayTest(vector3D startPos, vector3D endPos);
	btDiscreteDynamicsWorld* dynamicsWorld = nullptr;

protected:

	btBroadphaseInterface* broadphase = nullptr;
	btDefaultCollisionConfiguration* collisionConfiguration = nullptr;
	btCollisionDispatcher* dispatcher = nullptr;
	btSequentialImpulseConstraintSolver* solver = nullptr;
};

#endif //PHYSICSWORLD_HPP
