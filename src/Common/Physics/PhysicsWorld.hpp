#ifndef PHYSICSWORLD_HPP
#define PHYSICSWORLD_HPP

#include "Common/Physics/Bullet_Include.hpp"
#include "Common/Physics/RigidBody.hpp"
#include "Common/Entity/Entity.hpp"

//Prototype Classes
class World;

/*struct SingleRayTestResult
{
	Entity* hitEntity = nullptr;
	bool hasHit = false;
	const btRigidBody* hitBody;
	vector3D hitPosition;
	vector3D hitNormal;
	int index = 0;
	int userValue = 0;
};*/

class PhysicsWorld
{

public:
	PhysicsWorld(World* world);
	virtual ~PhysicsWorld();

	void update(double deltaTime);
	void addRigidBody(RigidBody* rigidBody);
	void removeRigidBody(RigidBody* rigidBody);

	//SingleRayTestResult singleRayTest(vector3D startPos, vector3D endPos);
	//SingleRayTestResult singleRayTestNotMe(vector3D startPos, vector3D endPos, btCollisionObject* me);
	btDiscreteDynamicsWorld* dynamicsWorld = nullptr;

protected:

private:
	World* parent = nullptr;

	btBroadphaseInterface* broadphase = nullptr;
	btDefaultCollisionConfiguration* collisionConfiguration = nullptr;
	btCollisionDispatcher* dispatcher = nullptr;
	btSequentialImpulseConstraintSolver* solver = nullptr;
};

#endif //PHYSICSWORLD_HPP
