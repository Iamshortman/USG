#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "Common/GLM_Include.hpp"

class Transform
{
public:
	Transform(vector3D position = vector3D(0.0), quaternionD orientation = quaternionD(1.0, 0.0, 0.0, 0.0));

	vector3D position;
	quaternionD orientation;

	//Getters for the directional vectors.
	vector3D getPosition() const;
	vector3D getForward() const;
	vector3D getUp() const;
	vector3D getLeft() const;
	vector3D getDirection(int i) const;
	quaternionD getOrientation() const;

	void setPosition(const vector3D& vec);
	void setOrientation(const quaternionD& quat);
	void setTransform(const Transform& transform);
	void setPositionAndRotationFromTransform(const Transform& transform);
	Transform transformBy(const Transform&  transform1) const;
	Transform untransformBy(const Transform & transform1) const;

	matrix4 getModleMatrix() const;
	matrix4 getModleMatrix(vector3D cameraPos, double divisorPosScale = 1.0) const;
	matrix4 getViewMatrix(vector3D cameraPos) const;
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