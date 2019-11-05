#pragma once
#include "hwlib.hpp"
#include "rtos.hpp"
#include <array>

struct IntChar
{
	IntChar() {}
	IntChar(bool isChar, int i, char c) : initialized(true), isChar(isChar), i(i), c(c) { }
	bool initialized = false;
	bool isChar;
	int i;
	char c;
};

class Oled_Display final : public rtos::task<>
{
public:
	Oled_Display(hwlib::glcd_oled & window, hwlib::terminal_from & textWriter) :
		task(8, "Oled_Display"),
		_displayFlag(this, "_displayFlag"),
		_displayPool("_displayPool"),
		_textWriter(textWriter),
		_window(window)
	{
	    
	}

	void clear()
	{
		_window.clear();
		_textWriter << "  \f";
	}

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

	void showNumber(int number)
	{
		std::array<IntChar, 100> temp;
		temp[0] = IntChar(false, number, 0); 
		_displayPool.write(temp);
		_displayFlag.set();
	}

	void newLine()
	{
		_textWriter << "\n";
	}

	void flush()
	{
		_textWriter << hwlib::flush;
	}

protected:
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