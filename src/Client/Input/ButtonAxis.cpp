#include "Client/Input/ButtonAxis.hpp"

#include <math.h>

ButtonAxis::ButtonAxis(InputButton* pos, InputButton* neg)
{
	this->positive = pos;
	this->negitive = neg;
}

ButtonAxis::~ButtonAxis()
{
	delete this->positive;
	delete this->negitive;
}

void ButtonAxis::update(double deltaTime)
{
	this->positive->update(deltaTime);
	this->negitive->update(deltaTime);

	this->axisValue = 0.0;

	if (positive->isButtonDown())
	{
		this->axisValue += this->range;
	}

	if (negitive->isButtonDown())
	{
		this->axisValue -= this->range;
	}

	if (this->inverted == true)
	{
		this->axisValue *= -1.0;
	}
}

double ButtonAxis::getAxis()
{
	return this->axisValue;
}

void ButtonAxis::setDeadzone(double deadzone)
{
	this->range = fabs(deadzone);
}

void ButtonAxis::setInverted(bool inverted)
{
	this->inverted = inverted;
}

double ButtonAxis::getDeadzone()
{
	return this->range;
}

bool ButtonAxis::getInverted()
{
	return this->inverted;
}