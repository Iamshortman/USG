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
	ConcaveMesh,
};

class CollisionShape : public ComponentNode
{
public:
	CollisionShape();
	~CollisionShape();

	void setBox(vector3D half_length);
	void setCapsule(double radius, double height);
	void setConvexMesh(string file_path);
	void setConcaveMesh(string file_path);

	btCollisionShape* getShape();

	void enable();
	void disable();

private:

	CollisionShapeType shape_type;
	btCollisionShape* shape = nullptr;

	int index;
};

#endif //COLLISIONSHAPE_HPP