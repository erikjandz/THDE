#ifndef FIREBUTTON_HPP
#define FIREBUTTON_HPP

#include "hwlib.hpp"

// This class serves as an interface for a hardware button which can be pressed
class FireButton
{
public:
	FireButton(hwlib::target::pin_in & pin) :
		_button(pin)
	{

	}

	// Returns true is the button is pressed
	bool isButtonPressed()
	{
		_button.refresh();
		return !_button.read();
	}

private:
	hwlib::target::pin_in _button;
};

#endif
