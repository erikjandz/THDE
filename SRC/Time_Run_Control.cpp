#include "Time_Run_Control.hpp"
#include "Oled_Display.hpp"
#include "Speaker.hpp"
#include "Hit_Transfer_Control.hpp"
#include "Hit_Run_Control.hpp"

void Time_Run_Control::setTime(int time)
{
	if(!_timeSet)
	{
		_timeSet = true;
		_time_set_pool.write(time*60);
		_time_set_flag.set();
	}
}

int Time_Run_Control::getTime()
{
	return _time_get_pool.read();
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
				_timeRemaining = _time_set_pool.read();
				_time_get_pool.write(_timeRemaining);
				_state = State::COUNTDOWN;
				break;
			}

			case State::COUNTDOWN:
				wait( _clock);
				
				_timeRemaining--;
				_time_get_pool.write(_timeRemaining);
				_display.clear();
				_display.showText("Time:");
				_display.newLine();
				_display.showNumber(_timeRemaining);
				_display.newLine();
				_display.showText("Score:");
				_display.newLine();
				_display.showNumber(_hitControl->getScore());
				_display.flush();
				
				// Check if game finished
				if(_timeRemaining <= 0)
				{
					_speaker.playEndTone();
					_state = State::IDLE;
				}

				break;
		}
	}
}