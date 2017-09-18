#ifndef KEYBOARDMOUSEDEVICE_HPP
#define KEYBOARDMOUSEDEVICE_HPP

#include "Client/SDL2_Include.hpp"
#include "Client/Input/InputDevice.hpp"


#include <string>
#include <unordered_map>
#include <vector>
using std::string;
using std::unordered_map;
using std::vector;

enum KeyboardMouse
{
	KEYBOARD,
	MOUSE,
};

enum MouseDirection
{
	Mouse_X,
	Mouse_Y,
};

struct MouseButtonValue
{
	uint8_t current_value = 0;
	uint8_t prev_value = 0;
	Uint32 timestamp = 0;
};

struct KeyboardButtonValue
{
	uint8_t current_value = 0;
	uint8_t prev_value = 0;
	Uint32 timestamp = 0;
};

struct MouseAxis
{
	MouseAxis(){}
	MouseAxis(MouseDirection direction, double sensitivity, double deadzone, bool inverted)
	{
		this->direction = direction;
		this->sensitivity = sensitivity;
		this->deadzone = deadzone;
		this->inverted = inverted;
	}

	MouseDirection direction;
	double sensitivity = 0.0;
	double deadzone = 0.0;
	bool inverted = false;
};

struct KeyboardMouseButton
{
	KeyboardMouseButton(KeyboardMouse device, int value)
	{
		this->device = device;
		this->value = value;
	}

	KeyboardMouse device;
	int value;
};

class KeyboardMouseDevice : public InputDevice
{
public:
	KeyboardMouseDevice();
	~KeyboardMouseDevice();

	void processEvent(SDL_Event event);
	void resetPreviousValues();

	void addAxis(string name, MouseAxis axis);
	void addButton(string name, KeyboardMouseButton button);

	bool hasAxis(string name);
	AxisReturn getAxis(string name);

	bool hasButton(string name);
	bool getButtonDown(string name);
	bool getButtonPressed(string name);

private:
	int32_t mouse_axis_state[2];
	uint32_t mouse_axis_timestamp;

	MouseButtonValue mouse_state[8];
	unordered_map<Uint32, KeyboardButtonValue> keyboard_state;

	unordered_map<string, MouseAxis> axis_bindings;
	unordered_map<string, vector<KeyboardMouseButton>> button_bindings;
};

#endif //KEYBOARDMOUSEDEVICE_HPP