#ifndef CYLINDERSHAPE_HPP
#define CYLINDERSHAPE_HPP

#include "Common/Physics/CollisionShapes/CollisionShape.hpp"
#include "BulletCollision/CollisionShapes/btCylinderShape.h"

class CylinderShape : public CollisionShape
{
public:
	CylinderShape(double radius, double height)
	{
		//this->btShape = new btCylinderShape(radius, height);
	};

};

#endif //CYLINDERSHAPE_HPP