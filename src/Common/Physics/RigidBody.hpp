#ifndef RIGIDBODY_HPP
#define RIGIDBODY_HPP

#include "Common/Physics/Bullet_Include.hpp"
#include "Common/Physics/CollisionShape.hpp"
#include "Common/GLM_Include.hpp"
#include "Common/Transform.hpp"

enum RigidBodyType
{
	SINGLE,
	MULTI,
};

//Prototype Class
class PhysicsWorld;

class RigidBody
{
public:
	RigidBody();
	virtual ~RigidBody();

	void setMass(double massToAdd);
	double getMass();

	void setInertiaTensor(vector3D inertia);
	vector3D getInertiaTensor();
	void calcInertiaTensorFromShape();

	void Activate(bool activate);

	Transform getWorldTransform();
	void setWorldTransform(Transform transform);

	vector3D getLinearVelocity() const;
	void setLinearVelocity(vector3D velocity);

	vector3D getAngularVelocity() const;
	void setAngularVelocity(vector3D velocity);

	void applyForce(vector3D &force, vector3D &localPos);
	void applyImpulse(vector3D &impulse, vector3D &localPos);

	void applyCentralForce(vector3D &force);
	void applyCentralImpulse(vector3D &impulse);

	void applyTorque(vector3D &torque);
	void applyTorqueImpulse(vector3D &torque);

	void setDampening(double linear, double angular);

	inline btRigidBody* getRigidBody() { return this->rigidBody; };

	inline PhysicsWorld* getPhysicsWorld() { return this->physics_world; };
	inline void setPhysicsWorld(PhysicsWorld* world) { this->physics_world = world; };

	virtual RigidBodyType getType() = 0;

protected:
	void setCollisionShape(btCollisionShape* shape);

	double mass = 1.0;
	vector3D inertia = vector3D(1.0);

	btRigidBody* rigidBody = nullptr;

	PhysicsWorld* physics_world = nullptr;
};

#endif //RIGIDBODY_HPP