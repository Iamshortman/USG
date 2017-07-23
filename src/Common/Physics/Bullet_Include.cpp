#include "Common/Physics/Bullet_Include.hpp"

btVector3 toBtVec3(const vector3D& vec)
{
	return btVector3(vec.x, vec.y, vec.z);
}

vector3D toVec3(const btVector3& vec)
{
	return vector3D(vec.getX(), vec.getY(), vec.getZ());
}

btQuaternion toBtQuat(const quaternionD& quat)
{
	return btQuaternion(quat.x, quat.y, quat.z, quat.w);
}

quaternionD toQuat(const btQuaternion& quat)
{
	return quaternionD(quat.getW(), quat.getX(), quat.getY(), quat.getZ());
}

btTransform toBtTransform(const Transform& trans)
{
	return btTransform(toBtQuat(trans.orientation) ,toBtVec3(trans.position));
}

Transform toTransform(const btTransform& trans)
{
	return Transform(toVec3(trans.getOrigin()), toQuat(trans.getRotation()));
}