#ifndef COLLISIONSHAPE_HPP
#define COLLISIONSHAPE_HPP

#include "Common/Component/ComponentNode.hpp"
#include "Common/GLM_Include.hpp"

#include "BulletCollision/CollisionShapes/btCollisionShape.h"

#include "Common/Types.hpp"

enum CollisionShapeType
{
	Box,
	Capsule,
	ConvexMesh,
};

class CollisionShape : public ComponentNode
{
public:
	CollisionShape(I_Node* node);
	~CollisionShape();

	void setBox(vector3D half_length);
	void setCapsule(double radius, double height);
	void setConvexMesh(string file_path);

	btCollisionShape* getShape();

	virtual void enable() override;
	virtual void disable() override;

private:

	CollisionShapeType shape_type;
	btCollisionShape* shape = nullptr;

	int index;
};

#endif //COLLISIONSHAPE_HPP