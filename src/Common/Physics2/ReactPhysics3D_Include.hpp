#ifndef REACT_PHYSICS_3D_INCLUDE_HPP
#define REACT_PHYSICS_3D_INCLUDE_HPP

#include "reactphysics3d.h"
#include "Common/GLM_Include.hpp"

inline reactphysics3d::Vector3 toRecVec3(const vector3D& vec)
{
	return reactphysics3d::Vector3(vec.x, vec.y, vec.z);
};

vector3D toVec3(const reactphysics3d::Vector3& vec)
{
	return vector3D(vec.x, vec.y, vec.z);
};

#endif //REACT_PHYSICS_3D_INCLUDE_HPP