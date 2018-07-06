#include "Transform.hpp"

	Transform::Transform(vector3D position, quaternionD orientation)
	{
		this->position = position;
		this->orientation = orientation;
	}

	//Getters for the directional vectors.
	vector3D Transform::getDirection(int i) const
	{
		if (i == 0)
		{
			return this->getLeft();
		}
		else if (i == 1)
		{
			return this->getUp();
		}
		else if (i == 2)
		{
			return this->getForward();
		}

		return vector3D(0.0);
	}

	matrix4 Transform::getModleMatrix() const
	{
		return glm::translate(matrix4(1.0F), (vector3F)this->position) * glm::toMat4((quaternionF)this->orientation);
	}

	matrix4 Transform::getModleMatrix(vector3D cameraPos, double divisorPosScale) const
	{
		matrix4 positionMatrix = matrix4();
		matrix4 rotationMatrix = matrix4();
		matrix4 scaleMatrix = matrix4();

		positionMatrix = glm::translate(matrix4(1.0F), (vector3F)((this->position - cameraPos) / divisorPosScale));
		rotationMatrix = glm::toMat4((quaternionF)this->orientation);
		scaleMatrix = glm::scale(matrix4(1.0F), vector3F((float)(1.0 / divisorPosScale)));

		return positionMatrix * rotationMatrix * scaleMatrix;
	}

	matrix4 Transform::getOriginViewMatrix() const
	{
		return glm::lookAt(vector3F(0.0), (vector3F)this->getForward(), (vector3F)this->getUp());
	}

	matrix3 Transform::getNormalMatrix() const
	{
		return glm::toMat3((quaternionF)this->orientation);
	}


	Transform Transform::transformBy(const Transform& transform1) const
	{
		Transform result;
		result.setOrientation(transform1.getOrientation() * this->getOrientation());
		result.setPosition(transform1.getPosition() + (transform1.getOrientation() * this->getPosition()));
		return result;
	}
