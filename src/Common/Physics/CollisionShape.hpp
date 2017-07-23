#ifndef COLLISIONSHAPE_HPP
#define COLLISIONSHAPE_HPP

#include "BulletCollision/CollisionShapes/btCollisionShape.h"

class CollisionShape
{
public:
	CollisionShape()
	{
	
	};

	~CollisionShape()
	{
		delete this->btShape;
	}

	btCollisionShape* btShape = nullptr;
};

#endif //COLLISIONSHAPE_HPP