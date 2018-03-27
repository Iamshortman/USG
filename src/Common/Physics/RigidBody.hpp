#ifndef RIGIDBODY_HPP
#define RIGIDBODY_HPP

#include "Common/Component.hpp"

#include <unordered_map>

#include "Common/Physics/Bullet_Include.hpp"
#include "Common/Physics/CollisionShape.hpp"
#include "Common/GLM_Include.hpp"
#include "Common/Transform.hpp"

//Prototype Class
class PhysicsWorld;

typedef unsigned int childId;

struct ChildShape
{
	CollisionShape* shape;
	double mass;
	Transform transform;
	childId index;
	GameObject* gameObject;
};

class RigidBody : public Component
{
public:
	RigidBody();
	virtual ~RigidBody();

	virtual void enable() override;
	virtual void disable() override;

	childId addChildShape(CollisionShape* shape, Transform transform, double mass, GameObject* node = nullptr);
	GameObject* getChildNode(childId id);
	void removeChildShape(childId id);
	void updateChildTransform(childId id, Transform transform);
	void rebuildCompondShape();

	void setMass(double massToAdd);
	double getMass();

	void setInertiaTensor(vector3D inertia);
	vector3D getInertiaTensor();

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
	btRigidBody* rigidBody = nullptr;
	btCompoundShape* compoundShape = nullptr;
	std::unordered_map<int, ChildShape> childShapes;
	btEmptyShape* emptyShape = nullptr;

	double mass = 1.0;
	vector3D inertia = vector3D(0.0);

	PhysicsWorld* world = nullptr;

	childId getNextId();
};

#endif //RIGIDBODY_HPP