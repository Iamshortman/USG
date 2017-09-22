#ifndef BULLET_INCLUDE_HPP
#define BULLET_INCLUDE_HPP

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#include "Common/GLM_Include.hpp"
#include "Common/Transform.hpp"

btVector3 toBtVec3(const vector3D& vec);
vector3D toVec3(const btVector3& vec);

btQuaternion toBtQuat(const quaternionD& quat);
quaternionD toQuat(const btQuaternion& quat);

btTransform toBtTransform(const Transform& trans);
Transform toTransform(const btTransform& trans);


#endif //BULLET_INCLUDE_HPP