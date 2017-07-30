#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "Common/GLM_Include.hpp"

class Transform
{
public:
	Transform(){};
	Transform(vector3D position){ this->position = position; };
	Transform(vector3D position, quaternionD orientation){ this->position = position; this->orientation = orientation; };

	vector3D position;
	quaternionD orientation;
	vector3D scale = vector3D(1.0f); //Start scale with a default of 1.0f

	//Getters for the directional vectors.
	vector3D getPosition() const { return this->position; };
	vector3D getForward() const { return this->orientation * vector3D(0.0f, 0.0f, 1.0f); };
	vector3D getUp() const { return this->orientation * vector3D(0.0f, 1.0f, 0.0f); };
	vector3D getRight() const { return this->orientation * vector3D(-1.0f, 0.0f, 0.0f); };
	vector3D getLeft() const { return this->orientation * vector3D(1.0f, 0.0f, 0.0f); };
	quaternionD getOrientation() const { return this->orientation; };
	vector3D getScale() const { return scale; };

	void setPosition(const vector3D& vec){ this->position = vec; };
	void setOrientation(const quaternionD& quat){ this->orientation = quat; }
	void setScale(const vector3D& vec){ this->scale = vec; }

	void setTransform(const Transform& transform)
	{
		this->setPosition(getPosition());
		this->setOrientation(getOrientation());
		this->setScale(getScale());
	};

	void setPositionAndRotationFromTransform(const Transform& transform)
	{
		this->setPosition(getPosition());
		this->setOrientation(getOrientation());
	};

	matrix4 getModleMatrix() const
	{
		matrix4 positionMatrix = matrix4();
		matrix4 rotationMatrix = matrix4();
		matrix4 scaleMatrix = matrix4();

		positionMatrix = glm::translate(matrix4(1.0F), (vector3F)this->position);
		rotationMatrix = glm::toMat4((quaternionF)this->orientation);
		scaleMatrix = glm::scale(matrix4(1.0F), (vector3F)this->scale);

		return positionMatrix * rotationMatrix * scaleMatrix;
	};

	matrix4 getModleMatrix(vector3D cameraPos, double divisorPosScale = 1.0) const
	{
		matrix4 positionMatrix = matrix4();
		matrix4 rotationMatrix = matrix4();
		matrix4 scaleMatrix = matrix4();

		positionMatrix = glm::translate(matrix4(1.0F), (vector3F)((this->position - cameraPos) / divisorPosScale));
		rotationMatrix = glm::toMat4((quaternionF)this->orientation);
		scaleMatrix = glm::scale(matrix4(1.0F), (vector3F)(scale / divisorPosScale));

		return positionMatrix * rotationMatrix * scaleMatrix;
	};

	matrix3 getNormalMatrix() const
	{
		return glm::toMat3((quaternionF)this->orientation);
	};


	Transform transformBy(const Transform&  transform1) const
	{
		Transform result;
		result.setOrientation(transform1.getOrientation() * this->getOrientation());
		result.setPosition(transform1.getPosition() + (transform1.getOrientation() * this->getPosition()));

		return result;
	};

};

#endif //TRANSFORM_HPP