#ifndef RIGIDBODY_HPP
#define RIGIDBODY_HPP

#include <unordered_map>

#include "Common/Physics/Bullet_Include.hpp"
#include "Common/GLM_Include.hpp"
#include "Common/Component/Transform.hpp"
#include "Common/Physics/CollisionShape.hpp"

//Prototype Class
class PhysicsWorld;
class Entity;

struct ChildShape
{
	CollisionShape* shape;
	Transform transform;
	int index;
};

class RigidBody
{
public:
	RigidBody(Entity* entity, double mass);
	RigidBody(Entity* entity, double mass, CollisionShape* shape);
	virtual ~RigidBody();

	int addChildShape(CollisionShape* shape, Transform transform);
	void removeChildShape(int i);
	void updateChildTransform(int i, Transform transform);
	void updateChildShapes();

	void setMass(double massToAdd);
	double getMass();

	void setInertiaTensor(vector3D inertia);
	vector3D getInertiaTensor();

	/*void addToPhysicsWorld(PhysicsWorld* physicsWorld, Entity* entity, Transform worldTransform);
	void removeFromPhysicsWorld();
	bool isInPhysicsWorld();*/

	void Activate(bool activate);

	Transform getWorldTransform();
	Transform getWorldTransform(vector3D scale);
	void setWorldTransform(Transform transform);

	/*Transform getCOMTransform();
	void setCOMTransform(Transform transform);*/

	vector3D getLinearVelocity() const;
	void setLinearVelocity(vector3D velocity);

	vector3D getAngularVelocity() const;
	void setAngularVelocity(vector3D velocity);

	void applyCentralForce(vector3D force);
	void applyCentralImpulse(vector3D impulse);

	void applyTorque(vector3D torque);
	void applyTorqueImpulse(vector3D torque);

	void setDampening(double linear, double angular);

	PhysicsWorld* getPhysicsWorld() const;

	btRigidBody* getRigidBody();

private:
	Entity* parent = nullptr;

	btRigidBody* rigidBody = nullptr;
	btCompoundShape* compoundShape = nullptr;

	double mass = 1.0;
	vector3D inertia = vector3D(1.0);

	PhysicsWorld* world = nullptr;

	std::unordered_map<int, ChildShape> shapes;
	int nextId = 0;
};

#endif //RIGIDBODY_HPP