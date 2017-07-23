#ifndef MOUSEAXIS_HPP
#define MOUSEAXIS_HPP

#include "InputAxis.hpp"

enum MouseDirection
{
	Mouse_X,
	Mouse_Y
};

class MouseAxis : public InputAxis
{
public:
	MouseAxis(MouseDirection direction, double sensitivity, double deadzone, bool inverted);
	virtual void update(double deltaTime);
	virtual double getAxis();

	void setSensitivity(double sensitivity);
	double getSensitivity();

	virtual void setDeadzone(double deadzone);
	virtual double getDeadzone();

	virtual void setInverted(bool inverted);
	virtual bool getInverted();

private:
	MouseDirection direction;

	double sensitivity = 1.0;
	double deadzone = 0.0;
	bool inverted = false;

	double axisValue = 0;
};

#endif //MOUSEAXIS_HPP