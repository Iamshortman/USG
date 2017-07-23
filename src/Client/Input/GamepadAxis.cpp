#include "GamepadAxis.hpp"

#include <math.h>

GamepadAxis::GamepadAxis(SDL_GameController* device, SDL_GameControllerAxis axis, double deadzone, bool inverted)
{
	this->device = device;
	this->axis = axis;
	this->deadzone = deadzone;
	this->inverted = inverted;
}

void GamepadAxis::update(double deltaTime)
{
	this->axisValue = SDL_GameControllerGetAxis(this->device, this->axis);
}

double GamepadAxis::getAxis()
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

void GamepadAxis::setDeadzone(double deadzone)
{
	this->deadzone = deadzone;
}

void GamepadAxis::setInverted(bool inverted)
{
	this->inverted = inverted;
}

double GamepadAxis::getDeadzone()
{
	return this->deadzone;
}

bool GamepadAxis::getInverted()
{
	return this->inverted;
}