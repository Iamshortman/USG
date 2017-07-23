#include "InputManager.hpp"

#include "Client/Client.hpp"
#include "Client/SDL2_Include.hpp"

#include "Client/Input/ButtonAxis.hpp"

#include "Client/Input/MouseAxis.hpp"
#include "Client/Input/KeyboardButton.hpp"

#include "Client/Input/JoystickAxis.hpp"
#include "Client/Input/JoystickButton.hpp"
#include "Client/Input/GamepadAxis.hpp"
#include "Client/Input/GamepadButton.hpp"


InputManager* InputManager::instance = nullptr;

InputManager::InputManager()
{
	InputManager::instance = this;

	SDL_Init(SDL_INIT_GAMECONTROLLER && SDL_INIT_JOYSTICK && SDL_INIT_HAPTIC);
	
	this->devices = list<InputDevice*>();

	this->devices.push_back(new InputDevice("Keyboard", KEYBOARD_MOUSE, nullptr));
	InputDevice* keyboard = this->devices.front();

	keyboard->addButton("DebugForward", new KeyboardButton(SDL_SCANCODE_W));
	keyboard->addAxis("DebugForwardBackward", new ButtonAxis(new KeyboardButton(SDL_SCANCODE_W), new KeyboardButton(SDL_SCANCODE_S)));
	keyboard->addAxis("DebugLeftRight", new ButtonAxis(new KeyboardButton(SDL_SCANCODE_A), new KeyboardButton(SDL_SCANCODE_D)));
	keyboard->addAxis("DebugUpDown", new ButtonAxis(new KeyboardButton(SDL_SCANCODE_SPACE), new KeyboardButton(SDL_SCANCODE_LSHIFT)));

	keyboard->addAxis("DebugPitch", new MouseAxis(MouseDirection::Mouse_Y, 10.0, 0.01, false));
	keyboard->addAxis("DebugYaw", new MouseAxis(MouseDirection::Mouse_X, 10.0, 0.01, true));
	keyboard->addAxis("DebugRoll", new ButtonAxis(new KeyboardButton(SDL_SCANCODE_E), new KeyboardButton(SDL_SCANCODE_Q)));
}

InputManager::~InputManager()
{
	for (InputDevice* device : this->devices)
	{
		delete device;
	}
	this->devices.clear();
}

void InputManager::update(double deltaTime)
{
	for (InputDevice* device : this->devices)
	{
		device->update(deltaTime);
	}

	this->centerMouse();
}

bool InputManager::hasAxis(string name)
{
	for (InputDevice* device : this->devices)
	{
		if (device->hasAxis(name))
		{
			return true;
		}
	}

	return false;
}

//TODO return axis for last changed instead?????
/*
	name: the axis name;
	return: the highest axis value(ignoring sign) of all devices; 
*/
double InputManager::getAxis(string name)
{
	double axisValue = 0.0;
	double tempAxisValue = 0.0;

	for (InputDevice* device : this->devices)
	{
		if (device->hasAxis(name))
		{
			tempAxisValue = device->getAxis(name);
			if (abs(axisValue) < abs(tempAxisValue))
			{
				axisValue = tempAxisValue;
			}
		}
	}

	return axisValue;
}

/*
	name: the button name;
	return: true if any devices have the button down;
*/
bool InputManager::getButtonDown(string name)
{
	for (InputDevice* device : this->devices)
	{
		if (device->hasButton(name) && device->getButtonDown(name))
		{
			return true;
		}
	}

	return false;
}

/*
	name: the button name;
	return: true if any devices have the button pressed;
*/
bool InputManager::getButtonPressed(string name)
{
	for (InputDevice* device : this->devices)
	{
		if (device->hasButton(name) && device->getButtonPressed(name))
		{
			return true;
		}
	}

	return false;
}

/*
	name: the button name;
	return: true if any devices have the button double pressed;
*/
bool InputManager::getButtonDoublePressed(string name)
{
	for (InputDevice* device : this->devices)
	{
		if (device->hasButton(name) && device->getButtonDoublePressed(name))
		{
			return true;
		}
	}

	return false;
}

void InputManager::processEvent(SDL_Event event)
{
	if (event.type == SDL_CONTROLLERDEVICEADDED)
	{
		int i = event.cdevice.which;

		if (SDL_IsGameController(i))
		{
			SDL_GameController* controller = SDL_GameControllerOpen(i);
			this->loadController(controller);
		}
		else
		{
			SDL_Joystick* joystick = SDL_JoystickOpen(i);
			this->loadJoystick(joystick); 
		}

		return;
	}
	else if(event.type == SDL_CONTROLLERDEVICEREMOVED)
	{
		int i = event.cbutton.which;

		if (SDL_IsGameController(i))
		{
			SDL_GameController* controller = SDL_GameControllerFromInstanceID(i);
			this->unloadController(controller);
			SDL_GameControllerClose(controller);
		}
		else
		{
			SDL_Joystick* joystick = SDL_JoystickFromInstanceID(i);
			this->unloadJoystick(joystick);
			SDL_JoystickClose(joystick);
		}

		return;
	}

}

void InputManager::loadController(SDL_GameController* controller)
{
	InputDevice* device = new InputDevice(SDL_GameControllerName(controller), InputType::GAMEPAD, controller);

	//Load Config

	this->devices.push_back(device);
}

void InputManager::unloadController(SDL_GameController* controller)
{
	InputDevice* device = nullptr;

	for (InputDevice* dev : this->devices)
	{
		if (dev->m_devicePtr == controller)
		{
			device = dev;
			break;
		}
	}

	if (device != nullptr)
	{
		this->devices.remove(device);
	}
}

void InputManager::loadJoystick(SDL_Joystick* joystick)
{
	InputDevice* device = new InputDevice(SDL_JoystickName(joystick), InputType::JOYSTICK, joystick);

	//Load Config

	this->devices.push_back(device);
}

void InputManager::unloadJoystick(SDL_Joystick* joystick)
{
	InputDevice* device = nullptr;

	for (InputDevice* dev : this->devices)
	{
		if (dev->m_devicePtr == joystick)
		{
			device = dev;
			break;
		}
	}

	if (device != nullptr)
	{
		this->devices.remove(device);
	}
}

void InputManager::centerMouse()
{
	if (this->mouseLocked == true)
	{
		int width, height;
		if (Client::instance != nullptr)
		{
			Client::instance->window->getWindowSize(width, height);
			Client::instance->window->setMousePos(width / 2, height / 2);
		}
	}
}

void InputManager::setMouseLock(bool locked)
{
	this->mouseLocked = locked;

	if (this->mouseLocked == true)
	{
		this->centerMouse();
		SDL_ShowCursor(SDL_DISABLE);
	}
	else
	{
		SDL_ShowCursor(SDL_ENABLE);
	}
}

bool InputManager::getMouseLock()
{
	return this->mouseLocked;
}