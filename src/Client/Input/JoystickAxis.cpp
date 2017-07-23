#include "JoystickAxis.hpp"

#include <math.h>

JoystickAxis::JoystickAxis(SDL_Joystick* device, int axis, double deadzone, bool inverted)
{
	this->device = device;
	this->axis = axis;
	this->deadzone = deadzone;
	this->inverted = inverted;
}

void JoystickAxis::update(double deltaTime)
{
	this->axisValue = SDL_JoystickGetAxis(this->device, this->axis);
}

double JoystickAxis::getAxis()
{
	double range = 1.0 - this->deadzone;

	//Get between -1 and 1
	double value = ((double)this->axisValue) / 32767.0;

	//apply deadzone
	if (abs(value) > this->deadzone)
	{
		double sign = value / abs(value);
		value = abs(value) - this->deadzone;
		value /= range;

		value *= sign;

		if (this->inverted)
		{
			value *= -1.0;
		}

		return value;
	}

	return 0.0;
}

void JoystickAxis::setDeadzone(double deadzone)
{
	this->deadzone = deadzone;
}

void JoystickAxis::setInverted(bool inverted)
{
	this->inverted = inverted;
}

double JoystickAxis::getDeadzone()
{
	return this->deadzone;
}

bool JoystickAxis::getInverted()
{
	return this->inverted;
}