#ifndef JOYSTICKBUTTON_HPP
#define JOYSTICKBUTTON_HPP

#include "InputButton.hpp"
#include "Client/SDL2_Include.hpp"

class KeyboardButton : public InputButton
{
public:
	KeyboardButton(int button);
	virtual void update(double deltaTime);
	virtual bool isButtonDown();
	virtual bool isButtonPressed();
	virtual bool isButtonDoublePressed();

private:
	int m_button = 0;

	Uint8 m_buttonValue = 0;
	Uint8 m_buttonValuePrev = 0;
};

#endif //JOYSTICKBUTTON_HPP