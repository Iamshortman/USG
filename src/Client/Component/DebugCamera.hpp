#ifndef PLAYERCONTROL_HPP
#define PLAYERCONTROL_HPP

#include "Common/Component.hpp"

struct DebugCamera : public Component
{
	DebugCamera(double linear, double angular);

	//meter per second
	double linearSpeed;

	//rad per second
	double angularSpeed;

	virtual void update(double delta_time);
};

#endif //PLAYERCONTROL_HPP
