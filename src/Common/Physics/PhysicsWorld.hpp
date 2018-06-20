#ifndef PHYSICSWORLD_HPP
#define PHYSICSWORLD_HPP

#include "Common/Physics/Bullet_Include.hpp"
#include "Common/Physics/RigidBody.hpp"
#include "Common/Entity/Node.hpp"

#include <set>

struct SingleRayTestResult
{
	bool hasHit = false;

	Entity* entity = nullptr;
	Node* node = nullptr;

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
	SingleRayTestResult singleRayTestNotMe(vector3D startPos, vector3D endPos, Entity* me);
	btDiscreteDynamicsWorld* dynamicsWorld = nullptr;

protected:
	//TODO: Remove this list
	std::set<RigidBody*> rigid_bodies;

	btBroadphaseInterface* broadphase = nullptr;
	btDefaultCollisionConfiguration* collisionConfiguration = nullptr;
	btCollisionDispatcher* dispatcher = nullptr;
	btSequentialImpulseConstraintSolver* solver = nullptr;
};

#endif //PHYSICSWORLD_HPP
