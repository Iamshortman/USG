#include "JoystickDevice.hpp"

JoystickDevice::JoystickDevice(string name, SDL_Joystick* devicePtr)
{
	this->name = name;
	this->devicePtr = devicePtr;

}

JoystickDevice::~JoystickDevice()
{

}

void JoystickDevice::processEvent(SDL_Event event)
{
	if (event.type == SDL_JOYAXISMOTION)
	{
		int axisID = event.jaxis.axis;

		this->axis_values[axisID].current_value = event.jaxis.value;
		this->axis_values[axisID].timestamp = event.jaxis.timestamp;
	}
	else if (event.type == SDL_JOYBUTTONDOWN || event.type == SDL_JOYBUTTONUP)
	{
		int buttonID = event.jbutton.button;

		this->button_values[buttonID].prev_value = button_values[buttonID].current_value;
		this->button_values[buttonID].current_value = event.jbutton.state;
		this->button_values[buttonID].timestamp = event.jbutton.timestamp;
	}
}

void JoystickDevice::resetPreviousValues()
{
	for (int i = 0; i < 32; i++)
	{
		this->button_values[i].prev_value = this->button_values[i].current_value;
	}
}

void JoystickDevice::addAxis(string name, JoystickAxis axis)
{
	if (!this->hasAxis(name))
	{
		this->axis_bindings[name] = vector<JoystickAxis>();
	}

	this->axis_bindings[name].push_back(axis);
}

void JoystickDevice::addButton(string name, JoystickButton button)
{
	if (!this->hasButton(name))
	{
		this->button_bindings[name] = vector<JoystickButton>();
	}

	this->button_bindings[name].push_back(button);
}

bool JoystickDevice::hasAxis(string name)
{
	return this->axis_bindings.find(name) != this->axis_bindings.end();
}

AxisReturn JoystickDevice::getAxis(string name)
{
	Uint16 timestamp = 0;
	JoystickAxis axis;

	if (hasAxis(name))
	{
		for (JoystickAxis temp_axis : axis_bindings[name])
		{
			if (this->axis_values[temp_axis.axisIndex].timestamp > timestamp)
			{
				axis = temp_axis;
				timestamp = this->axis_values[temp_axis.axisIndex].timestamp;
			}
		}

		double range = 1.0 - axis.deadzone;

		//Get between -1 and 1
		double value = ((double)this->axis_values[axis.axisIndex].current_value) / 32767.0;

		//apply deadzone
		if (fabs(value) > axis.deadzone)
		{
			double sign = value / fabs(value);
			value = fabs(value) - axis.deadzone;
			value /= range;

			value *= sign;

			if (axis.inverted)
			{
				value *= -1.0;
			}

			return AxisReturn(value, timestamp);
		}
	}

	return AxisReturn(0.0, timestamp);
}

bool JoystickDevice::hasButton(string name)
{
	return this->button_bindings.find(name) != this->button_bindings.end();
}

bool JoystickDevice::getButtonDown(string name)
{
	if (hasButton(name))
	{
		for (JoystickButton temp_button : this->button_bindings[name])
		{
			JoystickButtonValue value = this->button_values[temp_button.buttonIndex];
			if (value.current_value == SDL_PRESSED)
			{
				return true;
			}
		}
	}

	return false;
}

bool JoystickDevice::getButtonPressed(string name)
{
	if (hasButton(name))
	{
		for (JoystickButton temp_button : this->button_bindings[name])
		{
			JoystickButtonValue value = this->button_values[temp_button.buttonIndex];
			if (value.current_value == SDL_PRESSED && value.prev_value == SDL_RELEASED)
			{
				return true;
			}
		}
	}

	return false;
}