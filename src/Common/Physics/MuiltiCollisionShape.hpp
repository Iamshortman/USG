#ifndef MUILTI_COLLISIONSHAPE_HPP
#define MUILTI_COLLISIONSHAPE_HPP

#include "Common/Physics/CollisionShape.hpp"
#include <btBulletCollisionCommon.h>

struct MuiltiCollisionShape
{
	btCompoundShape* compound_shape = nullptr;
};

#endif //MUILTI_COLLISIONSHAPE_HPP