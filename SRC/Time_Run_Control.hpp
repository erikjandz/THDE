#pragma once
#include "hwlib.hpp"
#include "rtos.hpp"
#include "Oled_Display.hpp"
#include <cstring>

class Time_Run_Control : public rtos::task<>
{
public:
	Time_Run_Control(Oled_Display & display, Speaker & speaker, Hit_Transfer_Control & hit_transfer_control):
		task("Time_Run_Control"),
		_clock(this, 1'000'000, "_clock"),
		_time_set_flag(this, "flag"),
		_time_set_pool(this, "pool"),
		_display(display),
		_speaker(speaker),
		_hit_transfer_control(hit_transfer_control)
	{

	}

	void setTime(int time)
	{
		_time_set_pool.read();
		_time_set_flag.set();
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
				  wait( _time_set_flag);
					_state = State::INIT;
					break;
				case State::INIT:
					_timeRemaining = _time_set_pool.read();
					_state = State::COUNTDOWN;
					break;
				case State::COUNTDOWN:
					wait( _clock);
					_state = State::ACTIVE;
					break;
				case State::ACTIVE:
					_timeRemaining--;
					_display.clear();
					_display.showNumber(_timeRemaining);
					_display.showText("Time:");
					_display.flush();
					_state = State::IDLE;
					if(_timeRemaining <= 0){
						_hit_transfer_control.TimeFlagSet();
						_state = State::done;
					}
					break;
				case State::DONE:
					_speaker.playEndTone();
					hwlib::wait_ms(1000000)
					break;
			}
		}
	}

private:
	int _timeRemaining = -1;

	rtos::clock _clock;
	rtos::pool< int > _time_set_pool;
	rtos::flag _time_set_flag;

	enum class State { IDLE, ACTIVE, INIT, COUNTDOWN, DONE};
	State _state = State::IDLE;

	Oled_Display & _display;
	Speaker & _speaker;
	Hit_Transfer_Control & _hit_transfer_control;
};
