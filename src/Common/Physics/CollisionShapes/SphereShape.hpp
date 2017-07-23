#ifndef SPHERESHAPE_HPP
#define SPHERESHAPE_HPP

#include "Common/Physics/CollisionShapes/CollisionShape.hpp"
#include "BulletCollision/CollisionShapes/btSphereShape.h"

class SphereShape : public CollisionShape
{
public:
	SphereShape(double radius)
	{
		this->btShape = new btSphereShape(radius);
	};

};

#endif //SPHERESHAPE_HPP