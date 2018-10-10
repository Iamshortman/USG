#ifndef REACT_PHYSICS_3D_INCLUDE_HPP
#define REACT_PHYSICS_3D_INCLUDE_HPP

//Ignore Warnings
#pragma warning(disable : 4996)
#pragma warning(push, 0) 
#include "reactphysics3d.h"
#pragma warning(pop)

#include "Common/GLM_Include.hpp"
#include "Common/Transform.hpp"

inline reactphysics3d::Vector3 toRecVec3(const vector3D& vec)
{
	return reactphysics3d::Vector3((reactphysics3d::decimal)vec.x, (reactphysics3d::decimal)vec.y, (reactphysics3d::decimal)vec.z);
};

inline vector3D toGlmVec3(const reactphysics3d::Vector3& vec)
{
	return vector3D((double) vec.x, (double)vec.y, (double)vec.z);
}

inline reactphysics3d::Quaternion toRecQuat(const quaternionD& quat)
{
	return reactphysics3d::Quaternion((reactphysics3d::decimal)quat.x, (reactphysics3d::decimal)quat.y, (reactphysics3d::decimal)quat.z, (reactphysics3d::decimal)quat.w);
};

inline quaternionD toGlmQuat(const reactphysics3d::Quaternion& quat)
{
	return quaternionD((double)quat.w, (double)quat.x, (double)quat.y, (double)quat.z);
}

inline reactphysics3d::Transform toRecTrans(const Transform& trans)
{
	return reactphysics3d::Transform(toRecVec3(trans.position), toRecQuat(trans.orientation));
};

inline Transform toGlmTrans(const reactphysics3d::Transform& trans)
{
	reactphysics3d::Vector3 vector = trans.getPosition();
	reactphysics3d::Quaternion quat = trans.getOrientation();
	return Transform(toGlmVec3(vector), toGlmQuat(quat));
}


#endif //REACT_PHYSICS_3D_INCLUDE_HPP