#pragma once
#include "hwlib.hpp"

class FireButton
{
public:
	FireButton(hwlib::target::pin_in & pin) :
		_button(pin)
	{

	}

	bool isButtonPressed()
	{
		_button.refresh();
		return !_button.read();
	}

private:
	hwlib::target::pin_in _button;
};