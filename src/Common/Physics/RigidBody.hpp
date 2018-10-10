#ifndef RIGIDBODY_HPP
#define RIGIDBODY_HPP

#include "Common/Physics/ReactPhysics3D_Include.hpp"
#include "Common/Transform.hpp"
#include "Common/Physics/CollisionShape.hpp"
#include "Common/EntityX_Include.hpp"

#include <map>

//prototype class
class PhysicsWorld;

class RigidBody
{
public:
	RigidBody();
	virtual ~RigidBody();

	bool isInWorld();

	void createRigidBody(PhysicsWorld* world, const Transform& transform, Entity::Id id);
	void destroyRigidBody();

	void setTransform(const Transform& transform);
	Transform getTransform();

	void setLinearVelocity(const vector3D& velocity);
	vector3D getLinearVelocity();

	void setAngularVelocity(const vector3D& velocity);
	vector3D getAngularVelocity();

	void setGravityEnabled(bool enabled);
	bool getGravityEnabled();

	void applyCenteredForce(vector3D force);
	void applyForce(vector3D world_position, vector3D force);
	void applyTorque(vector3D torque);

	void addShape(CollisionShape* shape, Entity::Id id);
	void removeShape(CollisionShape* shape);

	double getMass();

protected:
	void addShapeInternal(CollisionShape* shape, Entity::Id id);
	void removeShapeInternal(CollisionShape* shape);

	reactphysics3d::RigidBody* rigid_body = nullptr;
	PhysicsWorld* world = nullptr;

	std::map<CollisionShape*, Entity::Id> shapes;

	friend PhysicsWorld;
};

#endif //RIGIDBODY_HPP