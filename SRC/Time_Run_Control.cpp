#include "Time_Run_Control.hpp"
#include "Oled_Display.hpp"
#include "Speaker.hpp"
#include "Hit_Transfer_Control.hpp"
#include "Hit_Run_Control.hpp"

void Time_Run_Control::setTime(int time)
{
	_time_set_pool.write(time*60);
	_time_set_flag.set();
}

int Time_Run_Control::getTime()
{
	return _timeRemaining;
}


void Time_Run_Control::main()
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
					_transferControl->TimeFlagSet();
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