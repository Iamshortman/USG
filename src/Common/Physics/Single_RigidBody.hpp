#ifndef SINGLE_RIGIDBODY_HPP
#define SINGLE_RIGIDBODY_HPP

#include "Common/Physics/RigidBody.hpp"

class Single_RigidBody : public RigidBody
{
public:
	Single_RigidBody(Entity* entity);
	~Single_RigidBody();

	void setShape(CollisionShape* shape);
	CollisionShape* getShape();

private:
	CollisionShape* shape;

	btEmptyShape* empty_shape;
};


#endif //SINGLE_RIGIDBODY_HPP