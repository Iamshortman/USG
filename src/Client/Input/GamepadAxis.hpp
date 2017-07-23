#ifndef GAMEPADAXIS_HPP
#define GAMEPADAXIS_HPP

#include "InputAxis.hpp"
#include "Client/SDL2_Include.hpp"

class GamepadAxis : public InputAxis
{
public:
	GamepadAxis(SDL_GameController* device, SDL_GameControllerAxis axis, double deadzone, bool inverted);
	virtual void update(double deltaTime);
	virtual double getAxis();

	virtual void setDeadzone(double deadzone);
	virtual void setInverted(bool inverted);

	virtual double getDeadzone();
	virtual bool getInverted();

private:
	SDL_GameController* device = nullptr;
	SDL_GameControllerAxis axis;
	double deadzone = 0.0;
	bool inverted = false;

	int axisValue = 0;
};

#endif //GAMEPADAXIS_HPP