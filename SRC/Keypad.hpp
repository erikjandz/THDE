#ifndef KEYPAD_HPP
#define KEYPAD_HPP
#include "hwlib.hpp"

class Keypad
{
public:
  Keypad(hwlib::keypad< 16 > & keyPad) :
      _keyPad(keyPad)
  {

  }

  char getKeyPressed() { return _keyPad.getc(); }

private:
  hwlib::keypad<16> & _keyPad;
};

#endif
