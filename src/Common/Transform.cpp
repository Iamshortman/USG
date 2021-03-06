#include "Common/Transform.hpp"

	Transform::Transform(vector3D position, quaternionD orientation)
	{
		this->position = position;
		this->orientation = orientation;
	}

	//Getters for the directional vectors.
	vector3D Transform::getPosition() const
	{ 
		return this->position; 
	}

	vector3D Transform::getForward() const
	{ 
		return this->orientation * vector3D(0.0f, 0.0f, 1.0f); 
	}

	vector3D Transform::getUp() const 
	{ 
		return this->orientation * vector3D(0.0f, 1.0f, 0.0f); 
	}

	vector3D Transform::getLeft() const 
	{ 
		return this->orientation * vector3D(1.0f, 0.0f, 0.0f); 
	}

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

	quaternionD Transform::getOrientation() const 
	{ 
		return this->orientation; 
	}

	void Transform::setPosition(const vector3D& vec)
	{ 
		this->position = vec; 
	}

	void Transform::setOrientation(const quaternionD& quat)
	{ 
		this->orientation = glm::normalize(quat); 
	}

	void Transform::setTransform(const Transform& transform)
	{
		this->setPosition(getPosition());
		this->setOrientation(getOrientation());
	}

	void Transform::setPositionAndRotationFromTransform(const Transform& transform)
	{
		this->setPosition(getPosition());
		this->setOrientation(getOrientation());
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

	matrix4 Transform::getViewMatrix(vector3D cameraPos) const
	{
		vector3F pos = (vector3F)(this->position - cameraPos);

		return glm::lookAt(pos, (vector3F)this->getForward() + pos, (vector3F)this->getUp());
	}

	matrix4 Transform::getOriginViewMatrix() const
	{
		return glm::lookAt(vector3F(0.0), (vector3F)this->getForward(), (vector3F)this->getUp());
	}

	matrix3 Transform::getNormalMatrix() const
	{
		return glm::toMat3((quaternionF)this->orientation);
	}


	Transform Transform::transformBy(const Transform&  transform1) const
	{
		Transform result;
		result.setOrientation(transform1.getOrientation() * this->getOrientation());
		result.setPosition(transform1.getPosition() + (transform1.getOrientation() * this->getPosition()));
		return result;
	}

	Transform Transform::untransformBy(const Transform& transform1) const
	{
		Transform result;
		result.setOrientation(this->getOrientation() * glm::inverse(transform1.getOrientation()));
		result.setPosition((this->getPosition() - transform1.getPosition()) * transform1.getOrientation());
		return result;
	}