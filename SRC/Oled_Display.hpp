//          Copyright Youri de Vor, Erik de Zeeuw, Hugo Cornel, Matthijs Koelewijn 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef OLED_DISPLAY_HPP
#define OLED_DISPLAY_HPP
#include "hwlib.hpp"
#include "rtos.hpp"
#include <array>

// A C++ struct that can hold a character & an integer
struct IntChar
{
	IntChar() {}
	IntChar(bool isChar, int i, char c) : initialized(true), isChar(isChar), i(i), c(c) { }
	bool initialized = false;
	bool isChar;
	int i;
	char c;
};

// This class serves as an OLED display
class Oled_Display final : public rtos::task<>
{
public:
	// HWLIB window and text terminal needed by constructor
	Oled_Display(hwlib::glcd_oled & window, hwlib::terminal_from & textWriter) :
		task(8, "Oled_Display"),
		_displayFlag(this, "_displayFlag"),
		_displayPool("_displayPool"),
		_textWriter(textWriter),
		_window(window)
	{

	}

	// This function clears the whole display screen
	void clear()
	{
		_window.clear();
		_textWriter << "  \f";
	}

	// This function shows characters on the screen
	void showText(const char * text)
	{
		int i = 0;
		std::array<IntChar, 100> temp;

		while(text[i] != '\0')
		{
			temp[i] = IntChar(true, 0, text[i]);
			i++;
		}

		_displayPool.write(temp);
		_displayFlag.set();
	}

	// This function shows integers on the screen
	void showNumber(int number)
	{
		std::array<IntChar, 100> temp;
		temp[0] = IntChar(false, number, 0);
		_displayPool.write(temp);
		_displayFlag.set();
	}

	// This function shows newlines on the screen
	void newLine()
	{
		_textWriter << "\n";
	}

	// This function flushes all writes to the display
	void flush()
	{
		_textWriter << hwlib::flush;
	}

protected:
	// RTOS main
	void main() override
	{
		for(;;)
		{
			switch(_state)
			{
				case State::IDLE:
					wait(_displayFlag);
					_state = State::ACTIVE;
					break;

				case State::ACTIVE:
					for(auto & s : _displayPool.read())
					{
						if(s.initialized)
						{
							if(s.isChar)
							{
								_textWriter << s.c;
							}
							else
							{
								_textWriter << s.i;
							}
						}
						else
						{
							break;
						}
					}

					_state = State::IDLE;
					break;
			}
		}
	}

private:
	rtos::flag _displayFlag;
	rtos::pool<std::array<IntChar, 100>> _displayPool;

	hwlib::terminal_from & _textWriter;
	hwlib::glcd_oled & _window;

	enum class State { IDLE, ACTIVE };
	State _state = State::IDLE;
};

#endif
