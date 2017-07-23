#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

#include "InputDevice.hpp"

#include <string>
#include <vector>
#include <list>
#include <SDL2/SDL_gamecontroller.h>
#include <SDL2/SDL_events.h>

using std::string;
using std::vector;
using std::list;

class InputManager
{
public:
	//Instance for the Singleton design pattern;
	static InputManager* instance;

	InputManager();
	~InputManager();
	void update(double deltaTime);

	void addDevice(string name, InputDevice* device);
	void removeDevice(string name);

	bool hasAxis(string name);
	double getAxis(string name);

	bool getButtonDown(string name);
	bool getButtonPressed(string name);
	bool getButtonDoublePressed(string name);

	void processEvent(SDL_Event event);
	void loadController(SDL_GameController* controller);
	void unloadController(SDL_GameController* controller);
	void loadJoystick(SDL_Joystick* joystick);
	void unloadJoystick(SDL_Joystick* joystick);

	//MouseFunction
	void centerMouse();
	void setMouseLock(bool locked);
	bool getMouseLock();

private:
	list<InputDevice*> devices;

	bool mouseLocked = false;
};

#endif //INPUTMANAGER_HPP