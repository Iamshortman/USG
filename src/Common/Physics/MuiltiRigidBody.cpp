#include "MuiltiRigidBody.hpp"

#include "Common/Entity/I_Node.hpp"
#include "Common/Physics/PhysicsWorld.hpp"
#include "Common/Component/Mass.hpp"

MuiltiRigidBody::MuiltiRigidBody(Entity* entity)
: RigidBody(entity)
{
	this->empty_shape = new btEmptyShape();

	this->compound_shape = new btCompoundShape();

	btDefaultMotionState* motionState = new btDefaultMotionState();
	btRigidBody::btRigidBodyConstructionInfo boxRigidBodyCI(this->mass, motionState, this->empty_shape, btVector3(1.0, 1.0, 1.0));
	this->rigidBody = new btRigidBody(boxRigidBodyCI);

	this->rigidBody->setUserPointer(this->parent);
}

MuiltiRigidBody::~MuiltiRigidBody()
{
	delete this->empty_shape;
	delete this->compound_shape;

	RigidBody::~RigidBody();
}

void MuiltiRigidBody::addChildShape(I_Node* node)
{
	if (node->hasNodeComponent<CollisionShape>())
	{
		if (this->rigidBody->getCollisionShape() == this->empty_shape)
		{
			this->setCollisionShape(this->compound_shape);
		}

		CollisionShape* shape = node->getNodeComponent<CollisionShape>();

		this->compound_shape->addChildShape(toBtTransform(node->getRelativeTransform()), shape->getShape());
		shape->getShape()->setUserPointer(node);

		this->child_shapes[node] = shape;

		this->recalculateMassProperties();
	}
}

void MuiltiRigidBody::removeChildShape(I_Node* node)
{
	if (node->hasNodeComponent<CollisionShape>())
	{
		CollisionShape* shape = node->getNodeComponent<CollisionShape>();

		this->compound_shape->removeChildShape(shape->getShape());

		if (this->compound_shape->getNumChildShapes() <= 0)
		{
			this->setCollisionShape(this->empty_shape);
		}

		this->child_shapes.erase(node);

		this->recalculateMassProperties();
	}
}

void MuiltiRigidBody::updateChildShapeTransform(I_Node* node)
{
	if (node->hasNodeComponent<CollisionShape>())
	{
		CollisionShape* shape = node->getNodeComponent<CollisionShape>();

		this->compound_shape->removeChildShape(shape->getShape());
		this->compound_shape->addChildShape(toBtTransform(node->getRelativeTransform()), shape->getShape());
	}
}

void MuiltiRigidBody::recalculateMassProperties()
{
	double total_mass = 0.0;
	btVector3 inertia_tensor = btVector3(0.0, 0.0, 0.0);

	for (auto iterator : this->child_shapes)
	{
		if (iterator.first->hasNodeComponent<Mass>())
		{
			double mass = iterator.first->getNodeComponent<Mass>()->getMass();
			total_mass += mass;

			//Center Of Mass moment of inertia
			btVector3 temp_mass = btVector3(0.0, 0.0, 0.0);
			iterator.second->getShape()->calculateLocalInertia(mass, temp_mass);
			inertia_tensor += temp_mass;

			//Parallel Axis moment of inertia
			vector3D position = iterator.first->getRelativeTransform().getPosition();
			inertia_tensor += toBtVec3((position * position) * mass);
		}
	}

	this->rigidBody->setMassProps(total_mass, inertia_tensor);
	this->mass = total_mass;
}
