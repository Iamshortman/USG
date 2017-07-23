#ifndef DEBUGCAMERACOMPONENT_HPP
#define DEBUGCAMERACOMPONENT_HPP

#include "Common/Component/Component.hpp"
#include "Common/Entity/Entity.hpp"

class DebugCameraComponent : public Component
{
public:
	DebugCameraComponent(double linear, double angular);
	virtual ~DebugCameraComponent();

	virtual void update(double deltaTime);

private:
	//Meter per second
	double linearSpeed;

	//rad per second
	double angularSpeed;
};

#endif //DEBUGCAMERACOMPONENT_HPP