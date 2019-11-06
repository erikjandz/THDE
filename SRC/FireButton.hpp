//          Copyright Youri de Vor, Erik de Zeeuw, Hugo Cornel, Matthijs Koelewijn 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef FIREBUTTON_HPP
#define FIREBUTTON_HPP

#include "hwlib.hpp"

/// This class serves as an interface for a hardware button which can be pressed
class FireButton
{
public:
	// Constructor needs button pin
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
