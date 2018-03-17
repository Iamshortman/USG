#include "Common/Entity/Component.hpp"
#include "Common/Entity/EntityNode.hpp"
#include "Common/GLM_Include.hpp"

class ComponentRotate : public Component
{
public:
	ComponentRotate(quaternionD start, quaternionD end, double time)
	{
		this->start = start;
		this->end = end;
		this->time = time;
		this->runningTime = 0.0;
	};

	virtual ~ComponentRotate()
	{

	};

	virtual void update(double deltaTime) 
	{
		this->runningTime += deltaTime;

		if (this->runningTime >= this->time)
		{
			this->runningTime = 0.0;
		}

		double delta = this->runningTime / this->time;

		currentRotation = glm::lerp(this->start, this->end, delta);

		Transform transform = this->parent->getLocalTransform();
		transform.setOrientation(currentRotation);
		this->parent->setLocalTransform(transform);
	};

	virtual ComponentType getComponentType() const override { return ComponentType::DEFAULT; };

protected:
	quaternionD start;
	quaternionD end;
	quaternionD currentRotation;
	double time;
	double runningTime;
};