#ifndef PHYSICSWORLD_HPP
#define PHYSICSWORLD_HPP

#include "Common/Physics/Bullet_Include.hpp"
#include "Common/Physics/RigidBody.hpp"
#include <set>

struct SingleRayTestResult
{
	bool hasHit = false;
	Entity* gameObject = nullptr;
	const btRigidBody* hitBody;
	vector3D hitPosition;
	vector3D hitNormal;
	int userValue = 0;
	int bodyId = 0;
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
	SingleRayTestResult singleRayTestNotMe(vector3D startPos, vector3D endPos, Entity* me);
	btDiscreteDynamicsWorld* dynamicsWorld = nullptr;

protected:
	std::set<RigidBody*> rigid_bodies;

	btBroadphaseInterface* broadphase = nullptr;
	btDefaultCollisionConfiguration* collisionConfiguration = nullptr;
	btCollisionDispatcher* dispatcher = nullptr;
	btSequentialImpulseConstraintSolver* solver = nullptr;
};

#endif //PHYSICSWORLD_HPP
