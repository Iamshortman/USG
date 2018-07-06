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
	inline vector3D getPosition() const { return this->position; };
	inline vector3D getForward() const { return this->orientation * vector3D(0.0f, 0.0f, 1.0f); };
	inline vector3D getUp() const { return this->orientation * vector3D(0.0f, 1.0f, 0.0f); };
	inline vector3D getLeft() const { return this->orientation * vector3D(1.0f, 0.0f, 0.0f); };
	vector3D getDirection(int i) const;
	inline quaternionD getOrientation() const { return this->orientation; };

	inline void setPosition(const vector3D& vec) { this->position = vec; };
	inline void setOrientation(const quaternionD& quat) { this->orientation = quat; };
	inline void setTransform(const Transform& trans) { this->setPosition(trans.getPosition()); this->setOrientation(trans.getOrientation()); };
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