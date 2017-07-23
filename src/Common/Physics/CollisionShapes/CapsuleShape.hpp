#ifndef CAPSULESHAPE_HPP
#define CAPSULESHAPE_HPP

#include "Common/Physics/CollisionShapes/CollisionShape.hpp"
#include "Common/Physics/Bullet_Include.hpp"
#include "Common/GLM_Include.hpp"

class CapsuleShape : public CollisionShape
{
public:
	CapsuleShape(double radius, double height)
	{
		this->btShape = new btCapsuleShape(radius, height);
	};

};

#endif //CAPSULESHAPE_HPP