#ifndef COLLISIONSHAPE_HPP
#define COLLISIONSHAPE_HPP

#include "Common/GLM_Include.hpp"

#include "BulletCollision/CollisionShapes/btCollisionShape.h"

#include "Common/Types.hpp"

enum CollisionShapeType
{
	Box,
	Capsule,
	ConvexMesh,
};

class CollisionShape
{
public:
	CollisionShape();
	~CollisionShape();

	void setBox(vector3D half_length);
	void setCapsule(double radius, double height);
	void setConvexMesh(string file_path);

	btCollisionShape* getShape();

	virtual void enable();
	virtual void disable();

private:

	CollisionShapeType shape_type;
	btCollisionShape* shape = nullptr;

	int index;
};

#endif //COLLISIONSHAPE_HPP