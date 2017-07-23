#ifndef JOYSTICKAXIS_HPP
#define JOYSTICKAXIS_HPP

#include "InputAxis.hpp"
#include <SDL2\SDL_joystick.h>

class JoystickAxis: public InputAxis
{
public:
	JoystickAxis(SDL_Joystick* device, int axis, double deadzone, bool inverted);
	virtual void update(double deltaTime);
	virtual double getAxis();

	virtual void setDeadzone(double deadzone);
	virtual void setInverted(bool inverted);

	virtual double getDeadzone();
	virtual bool getInverted();

private:
	SDL_Joystick* device = nullptr;
	int axis;
	double deadzone = 0.0;
	bool inverted = false;

	int axisValue = 0;
};

#endif //JOYSTICKAXIS_HPP