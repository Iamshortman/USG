#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "Common/GLM_Include.hpp"

class Transform
{
public:
	Transform();
	Transform(vector3D position);
	Transform(vector3D position, quaternionD orientation);

	vector3D position;
	quaternionD orientation;

	//Getters for the directional vectors.
	vector3D getPosition() const;
	vector3D getForward() const;
	vector3D getUp() const;
	vector3D getRight() const;
	vector3D getLeft() const;
	quaternionD getOrientation() const;

	void setPosition(const vector3D& vec);
	void setOrientation(const quaternionD& quat);
	void setTransform(const Transform& transform);
	void setPositionAndRotationFromTransform(const Transform& transform);
	Transform transformBy(const Transform&  transform1) const;

	matrix4 getModleMatrix() const;
	matrix4 getModleMatrix(vector3D cameraPos, double divisorPosScale = 1.0) const;
	matrix4 getOriginViewMatrix() const;
	matrix3 getNormalMatrix() const;

	inline bool operator==(const Transform& other)
	{
		return this->position == other.position && this->orientation == other.orientation;
	}

	inline bool operator!=(const Transform& other)
	{
		return !(*this == other);
	}

};

#endif //TRANSFORM_HPP