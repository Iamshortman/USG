#include "Client/Input/KeyboardMouseDevice.hpp"
#include "Client/Client.hpp"

KeyboardMouseDevice::KeyboardMouseDevice()
{

}

KeyboardMouseDevice::~KeyboardMouseDevice()
{

}

void KeyboardMouseDevice::processEvent(SDL_Event event)
{
	if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
	{
		if (event.key.repeat == 0)
		{
			SDL_Scancode key = event.key.keysym.scancode;

			if (this->keyboard_state.find(key) == this->keyboard_state.end())
			{
				this->keyboard_state[key] = KeyboardButtonValue();
			}

			this->keyboard_state[key].prev_value = this->keyboard_state[key].current_value;
			this->keyboard_state[key].current_value = event.key.state;
			this->keyboard_state[key].timestamp = event.key.timestamp;
		}
	}
	else if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
	{
		Uint8 buttonID = event.button.button;

		this->mouse_state[buttonID].prev_value = this->mouse_state[buttonID].prev_value;
		this->mouse_state[buttonID].current_value = event.button.state;
		this->mouse_state[buttonID].timestamp = event.key.timestamp;
	}
	else if (event.type == SDL_MOUSEMOTION)
	{
		int xPos, yPos;
		Client::instance->window->getCenteredMousePos(xPos, yPos);
		this->mouse_axis_state[0] = -xPos;
		this->mouse_axis_state[1] = yPos;
		this->mouse_axis_timestamp = event.motion.timestamp;
	}
}

void KeyboardMouseDevice::resetPreviousValues()
{
	this->mouse_axis_state[0] = 0;
	this->mouse_axis_state[1] = 0;

	for (int i = 0; i < 8; i++)
	{
		this->mouse_state[i].prev_value = this->mouse_state[i].current_value;
	}

	for (auto it : this->keyboard_state)
	{
		this->keyboard_state[it.first].prev_value = this->keyboard_state[it.first].current_value;
	}
}

void KeyboardMouseDevice::addAxis(string name, MouseAxis axis)
{
	if (!this->hasAxis(name))
	{
		this->axis_bindings[name] = axis;
	}
}

void KeyboardMouseDevice::addButton(string name, KeyboardMouseButton button)
{
	if (!this->hasButton(name))
	{
		this->button_bindings[name] = vector<KeyboardMouseButton>();
	}

	this->button_bindings[name].push_back(button);
}


bool KeyboardMouseDevice::hasAxis(string name)
{
	return this->axis_bindings.find(name) != this->axis_bindings.end();
}

AxisReturn KeyboardMouseDevice::getAxis(string name)
{
	AxisReturn axis;
	if (this->hasAxis(name))
	{
		axis.timestamp = mouse_axis_timestamp;
		MouseAxis mouse = this->axis_bindings[name];
		double value;
		if (mouse.direction == MouseDirection::Mouse_X)
		{
			value = (double)this->mouse_axis_state[0];
		}
		else
		{
			value = (double)this->mouse_axis_state[1];
		}

		if (abs(value) > mouse.deadzone)
		{
			double sign = value / abs(value);
			value = abs(value) - mouse.deadzone;
			value /= (1.0 - mouse.deadzone);

			value *= sign;

			if (mouse.inverted)
			{
				value *= -1.0;
			}

			value *= mouse.sensitivity;
			axis.value = value;
		}
	}

	return axis;
}


bool KeyboardMouseDevice::hasButton(string name)
{
	return this->button_bindings.find(name) != this->button_bindings.end();
}

bool KeyboardMouseDevice::getButtonDown(string name)
{
	if (hasButton(name))
	{
		for (KeyboardMouseButton temp_button : this->button_bindings[name])
		{
			if (temp_button.device == MOUSE)
			{
				if (this->mouse_state[temp_button.value].current_value == SDL_PRESSED)
				{
					return true;
				}
			}
			else if(temp_button.device == KEYBOARD)
			{
				if (this->keyboard_state[temp_button.value].current_value == SDL_PRESSED)
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool KeyboardMouseDevice::getButtonPressed(string name)
{
	if (hasButton(name))
	{
		for (KeyboardMouseButton temp_button : this->button_bindings[name])
		{
			if (temp_button.device == MOUSE)
			{
				if (this->mouse_state[temp_button.value].current_value == SDL_PRESSED && this->mouse_state[temp_button.value].prev_value == SDL_RELEASED)
				{
					return true;
				}
			}
			else if (temp_button.device == KEYBOARD)
			{
				if (this->keyboard_state[temp_button.value].current_value == SDL_PRESSED && this->keyboard_state[temp_button.value].prev_value == SDL_RELEASED)
				{
					return true;
				}
			}
		}
	}

	return false;
}
