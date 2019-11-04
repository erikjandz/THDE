#pragma once
#include "hwlib.hpp"
#include "rtos.hpp"
#include "Oled_Display.hpp"
#include "Hit_Run_Control.hpp"
#include <cstring>

class Time_Run_Control : public rtos::task<>
{
public:
	Time_Run_Control(Oled_Display & display, Hit_Run_Control & hitControl) :
		task("Time_Run_Control"),
		_clock(this, 1'000'000, "_clock"),
		_hitControl(hitControl),
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
					_display.newLine();
					_display.showText("Score:");
					_display.newLine();
					_hitControl.displayScore();
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

	Hit_Run_Control & _hitControl;

	Oled_Display & _display;
};