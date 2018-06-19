#ifndef MUILTI_RIGIDBODY_HPP
#define MUILTI_RIGIDBODY_HPP

#include "Common/Physics/RigidBody.hpp"
#include <map>


//Prototype TODO replace
class Node;

class MuiltiRigidBody : public RigidBody
{
public:
	MuiltiRigidBody(Entity* entity);
	~MuiltiRigidBody();

	void addChildShape(Node* node, CollisionShape* shape);
	void removeChildShape(Node* node);
	void updateChildShapeTransform(Node* node);

	virtual RigidBodyType getType() { return RigidBodyType::MULTI; };

private:
	struct ChildShape
	{
		CollisionShape* shape = nullptr;
		int shape_index;
	};

	std::map<Node*, ChildShape> child_shapes;

	btCompoundShape* compound_shape = nullptr;

	btEmptyShape* empty_shape = nullptr;
};


#endif //MUILTI_RIGIDBODY_HPP