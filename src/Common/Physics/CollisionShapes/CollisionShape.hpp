#ifndef COLLISIONSHAPE_HPP
#define COLLISIONSHAPE_HPP

#include "BulletCollision/CollisionShapes/btCollisionShape.h"

class CollisionShape
{
public:
	btCollisionShape* btShape = nullptr;
};

#endif //COLLISIONSHAPE_HPP