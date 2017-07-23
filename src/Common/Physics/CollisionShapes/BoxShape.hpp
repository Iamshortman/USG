#ifndef BOXSHAPE_HPP
#define BOXSHAPE_HPP

#include "Common/Physics/CollisionShapes/CollisionShape.hpp"
#include "Common/Physics/Bullet_Include.hpp"
#include "Common/GLM_Include.hpp"

class BoxShape : public CollisionShape
{
public:
	BoxShape(vector3D halfExtents)
	{
		this->btShape = new btBoxShape(toBtVec3(halfExtents));
	};

};

#endif //BOXSHAPE_HPP