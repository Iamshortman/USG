#include "Client/Input/KeyboardButton.hpp"

KeyboardButton::KeyboardButton(int button)
{
	m_button = button;
}

void KeyboardButton::update(double deltaTime)
{
	//TODO BetterWay
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	//Make new state the old state
	m_buttonValuePrev = m_buttonValue;

	m_buttonValue = state[m_button];
}

bool KeyboardButton::isButtonDown()
{
	if (m_buttonValue)
	{
		return true;
	}

	return false;
}

bool KeyboardButton::isButtonPressed()
{
	if (m_buttonValue && !m_buttonValuePrev)
	{
		return true;
	}

	return false;
}

bool KeyboardButton::isButtonDoublePressed()
{
	return false;
}