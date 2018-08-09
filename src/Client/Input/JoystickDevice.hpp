#ifndef JOYSTICKDEVICE_HPP
#define JOYSTICKDEVICE_HPP

#include "Client/SDL2_Include.hpp"
#include "Client/Input/InputDevice.hpp"


#include <string>
#include <unordered_map>
#include <vector>
using std::string;
using std::unordered_map;
using std::vector;

enum JoystickAxisRange
{
	FULL,
	FORWARD,
	BACKWARD
};

struct JoystickAxisValue
{
	int16_t current_value = 0;
	Uint32 timestamp = 0;
};

struct JoystickButtonValue
{
	uint8_t current_value = 0;
	uint8_t prev_value = 0;
	Uint32 timestamp = 0;
};

struct JoystickAxis
{
	JoystickAxis() {};
	JoystickAxis(int axisIndex, double deadzone = 0.0, bool inverted = false, JoystickAxisRange range = JoystickAxisRange::FULL)
	{
		this->axisIndex = axisIndex;
		this->deadzone = deadzone;
		this->inverted = inverted;
		this->range = range;
	}

	int axisIndex = 0;
	double deadzone = 0.0;
	bool inverted = false;
	JoystickAxisRange range = JoystickAxisRange::FULL;
};

struct JoystickButton
{
	JoystickButton() {};
	JoystickButton(int buttonIndex)
	{
		this->buttonIndex = buttonIndex;
	}

	int buttonIndex = 0;
};

class JoystickDevice : public InputDevice
{
public:
	JoystickDevice(string name, SDL_Joystick* devicePtr);
	~JoystickDevice();

	void processEvent(SDL_Event event);
	void resetPreviousValues();

	void addAxis(string name, JoystickAxis axis);
	void addButton(string name, JoystickButton button);

	bool hasAxis(string name);
	AxisReturn getAxis(string name);

	bool hasButton(string name);
	bool getButtonDown(string name);
	bool getButtonPressed(string name);

private:
	SDL_Joystick* devicePtr = nullptr;
	JoystickAxisValue axis_values[32];
	JoystickButtonValue button_values[32];

	unordered_map<string, vector<JoystickAxis>> axis_bindings;
	unordered_map<string, vector<JoystickButton>> button_bindings;
};

#endif //JOYSTICKDEVICE_HPP