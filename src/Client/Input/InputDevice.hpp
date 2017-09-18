#ifndef INPUTDEVICE_HPP
#define INPUTDEVICE_HPP

#include "Client/SDL2_Include.hpp"
#include <string>
using std::string;

struct AxisReturn
{
	AxisReturn() {}
	AxisReturn(double value, Uint32 timestamp)
	{
		this->value = value;
		this->timestamp = timestamp;
	}
	double value = 0.0;
	Uint32 timestamp = 0;
};

class InputDevice
{
public:
	string name;

	virtual void processEvent(SDL_Event event) = 0;
	virtual void update(double deltaTime) {};
	virtual void resetPreviousValues() = 0;
	
	virtual bool hasAxis(string name) = 0;
	virtual AxisReturn getAxis(string name) = 0;

	virtual bool hasButton(string name) = 0;
	virtual bool getButtonDown(string name) = 0;
	virtual bool getButtonPressed(string name) = 0;
};

#endif //INPUTDEVICE_HPP