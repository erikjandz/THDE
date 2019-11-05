#pragma once
#include "hwlib.hpp"
#include "rtos.hpp"
#include "Oled_Display.hpp"
#include "Speaker.hpp"
#include "Hit_Transfer_Control.hpp"
#include "Hit_Run_Control.hpp"
#include <cstring>

class Time_Run_Control : public rtos::task<>
{
public:
	Time_Run_Control(Oled_Display & display, Speaker & speaker):
		task("Time_Run_Control"),
		_clock(this, 1'000'000, "_clock"),
		_time_set_flag(this, "flag"),
		_time_set_pool("pool"),
		_display(display),
		_speaker(speaker)
		//_hit_transfer_control(hit_transfer_control)
	{

	}

	void giveHitControlPointer(Hit_Run_Control * instance)
	{
		_hitControl = instance;
	}

	void setTime(int time)
	{
		_time_set_pool.write(time);
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
				{
				  	wait( _time_set_flag);
					_state = State::INIT;
					break;
				}

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
					_display.showText("Time:");
					_display.newLine();
					_display.showNumber(_timeRemaining);
					_display.newLine();
					_display.showText("Score:");
					_display.newLine();
					_display.showNumber(_hitControl->getScore());
					_display.flush();
					_state = State::COUNTDOWN;
					if(_timeRemaining <= 0){
						//_hit_transfer_control.TimeFlagSet();
						_state = State::DONE;
					}
					break;

				case State::DONE:
					_speaker.playEndTone();
					hwlib::wait_ms(1000000);
					break;
			}
		}
	}

private:
	int _timeRemaining = -1;

	rtos::clock _clock;
	rtos::flag _time_set_flag;
	rtos::pool< int > _time_set_pool;

	enum class State { IDLE, ACTIVE, INIT, COUNTDOWN, DONE};
	State _state = State::IDLE;

	Oled_Display & _display;
	Speaker & _speaker;
	//Hit_Transfer_Control & _hit_transfer_control;
	Hit_Run_Control * _hitControl = nullptr;
};
