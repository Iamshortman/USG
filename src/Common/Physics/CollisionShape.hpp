#ifndef COLLISIONSHAPE_HPP
#define COLLISIONSHAPE_HPP

#include "Common/GLM_Include.hpp"

#include "BulletCollision/CollisionShapes/btCollisionShape.h"

enum CollisionShapeType
{
	Box,
	Capsule,
};

class CollisionShape
{
public:
	CollisionShape();
	~CollisionShape();

	void setBox(vector3D half_length);
	void setCapsule(double radius, double height);

	btCollisionShape* getShape();

private:

	CollisionShapeType shape_type;
	btCollisionShape* shape = nullptr;

	int index;
};

#endif //COLLISIONSHAPE_HPP