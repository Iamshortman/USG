#ifndef RIGIDBODY_HPP
#define RIGIDBODY_HPP

#include <unordered_map>

#include "Common/Physics/Bullet_Include.hpp"
#include "Common/Physics/CollisionShape.hpp"
#include "Common/GLM_Include.hpp"
#include "Common/Transform.hpp"

//Prototype Class
class PhysicsWorld;

class RigidBody
{
public:
	RigidBody(bool is_static = false);
	virtual ~RigidBody();

	int addChildShape(CollisionShape* shape);
	GameObject* getChildNode(int id);
	void removeChildShape(int id);

	/*void setMass(double massToAdd);
	double getMass();

	void setInertiaTensor(vector3D inertia);
	vector3D getInertiaTensor();*/

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

	btRigidBody* getRigidBody();

private:
	const bool is_static;

	btRigidBody* rigidBody = nullptr;
	btCompoundShape* compoundShape = nullptr;
	std::unordered_map<int, CollisionShape*> childShapes;
	btEmptyShape* emptyShape = nullptr;

	PhysicsWorld* world = nullptr;

	int getNextId();
};

#endif //RIGIDBODY_HPP