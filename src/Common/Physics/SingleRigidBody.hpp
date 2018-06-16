#ifndef SINGLE_RIGIDBODY_HPP
#define SINGLE_RIGIDBODY_HPP

#include "Common/Physics/RigidBody.hpp"

class SingleRigidBody : public RigidBody
{
public:
	SingleRigidBody(Entity* entity);
	~SingleRigidBody();

	void setShape(CollisionShape* shape);
	CollisionShape* getShape();

	virtual RigidBodyType getType() { return RigidBodyType::SINGLE; };

private:
	CollisionShape* shape;

	btEmptyShape* empty_shape;
};


#endif //SINGLE_RIGIDBODY_HPP