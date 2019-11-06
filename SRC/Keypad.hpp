//          Copyright Youri de Vor, Erik de Zeeuw, Hugo Cornel, Matthijs Koelewijn 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef KEYPAD_HPP
#define KEYPAD_HPP
#include "hwlib.hpp"

// This class serves as a keypad which can return certain key presses
class Keypad
{
public:
	// HWLIB keypad instance needed by constructor
	  Keypad(hwlib::keypad< 16 > & keyPad) :
	      _keyPad(keyPad)
	  {

	  }

	  // Return 1 of 16 keys pressed on the keypad
	  char getKeyPressed() { return _keyPad.getc(); }

private:
  	hwlib::keypad<16> & _keyPad;
};

#endif
