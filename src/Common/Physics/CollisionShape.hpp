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

struct CollisionShape
{
	CollisionShape(btCollisionShape* shape, CollisionShapeType type) : shape(shape), type(type) {};

	const btCollisionShape* shape;
	const CollisionShapeType type;
};

namespace CollisionShapes
{
	CollisionShape createBox(vector3D half_length);
	CollisionShape createCapsule(double radius, double height);
	CollisionShape createConvexMesh(string file_path);
};

#endif //COLLISIONSHAPE_HPP