#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

#include "Client/Input/InputDevice.hpp"
#include "Client/Input/KeyboardMouseDevice.hpp"

#include <string>
#include <unordered_map>
#include <SDL2/SDL_gamecontroller.h>
#include <SDL2/SDL_events.h>

using std::string;
using std::unordered_map;

class InputManager
{
public:
	//Instance for the Singleton design pattern;
	static InputManager* instance;

	InputManager();
	~InputManager();
	void update(double deltaTime);
	void resetPreviousValues();

	bool hasAxis(string name);
	double getAxis(string name);

	bool getButtonDown(string name);
	bool getButtonPressed(string name);

	double getButtonAxisCombo(string axis_name, string pos_button_name, string neg_button_name);

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
	KeyboardMouseDevice* keyboardMouse = nullptr;
	unordered_map<void*, InputDevice*> device_map;

	bool mouseLocked = false;
};

#endif //INPUTMANAGER_HPP