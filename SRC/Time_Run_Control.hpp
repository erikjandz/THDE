#pragma once
#include "hwlib.hpp"
#include "rtos.hpp"
#include "Oled_Display.hpp"
#include <cstring>

class Time_Run_Control : public rtos::task<>
{
public:
	Time_Run_Control(Oled_Display & display) :
		task("Time_Run_Control"),
		_clock(this, 1'000'000, "_clock"),
		_display(display)
	{

	}

	void setTime(int time)
	{
		_timeRemaining = time;
	}

	int getTime()
	{
		return _timeRemaining;
	}

protected:
	void main() override
	{
		for(;;)
		{
			switch(_state)
			{
				case State::IDLE:
					wait(_clock);
					_state = State::ACTIVE;
					break;

				case State::ACTIVE:
					_timeRemaining--;

					_display.clear();
					_display.showNumber(_timeRemaining);
					_display.showText("Time:");
					_display.flush();
					_state = State::IDLE;
					break;
			}
		}
	}

private:
	int _timeRemaining = 25;

	rtos::clock _clock;

	enum class State { IDLE, ACTIVE };
	State _state = State::IDLE;

	Oled_Display & _display;
};