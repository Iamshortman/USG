#ifndef BUTTONAXIS_HPP
#define BUTTONAXIS_HPP

#include "Client/Input/InputAxis.hpp"
#include "Client/Input/InputButton.hpp"

class ButtonAxis: public InputAxis
{
public:
	ButtonAxis(InputButton* pos, InputButton* neg);
	virtual ~ButtonAxis();
	virtual void update(double deltaTime);
	virtual double getAxis();

	virtual void setDeadzone(double deadzone);
	virtual void setInverted(bool inverted);

	virtual double getDeadzone();
	virtual bool getInverted();

private:
	InputButton* positive;
	InputButton* negitive;

	double range = 1.0;
	bool inverted = false;

	double axisValue = 0;
};

#endif //BUTTONAXIS_HPP