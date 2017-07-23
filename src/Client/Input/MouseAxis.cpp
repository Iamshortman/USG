#include "MouseAxis.hpp"

#include "Client/Client.hpp"
#include "Client/SDL2_Include.hpp"
#include "Client/Input/InputManager.hpp"

#include <math.h>

MouseAxis::MouseAxis(MouseDirection direction, double sensitivity, double deadzone, bool inverted)
{
	this->direction = direction;
	this->sensitivity = sensitivity;
	this->deadzone = deadzone;
	this->inverted = inverted;
}

void MouseAxis::update(double deltaTime)
{
	double temp;
	if (this->direction == MouseDirection::Mouse_X)
	{
		Client::instance->window->getCenteredMousePos(this->axisValue, temp);
	}
	else
	{
		Client::instance->window->getCenteredMousePos(temp, this->axisValue);
	}
}

double MouseAxis::getAxis()
{

	if (InputManager::instance->getMouseLock())
	{
		double range = 1.0 - this->deadzone;

		double value = this->axisValue;

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

			return value * this->sensitivity;
		}
	}

	return 0.0;
}

void MouseAxis::setSensitivity(double sensitivity)
{
	this->sensitivity = sensitivity;
}

double MouseAxis::getSensitivity()
{
	return this->sensitivity;
}

void MouseAxis::setDeadzone(double deadzone)
{
	this->deadzone = deadzone;
}

double MouseAxis::getDeadzone()
{
	return this->deadzone;
}

void MouseAxis::setInverted(bool inverted)
{
	this->inverted = inverted;
}

bool MouseAxis::getInverted()
{
	return this->inverted;
}