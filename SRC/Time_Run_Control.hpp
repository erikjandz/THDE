//          Copyright Youri de Vor, Erik de Zeeuw, Hugo Cornel, Matthijs Koelewijn 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef TIME_RUN_CONTROL_HPP
#define TIME_RUN_CONTROL_HPP
#include "hwlib.hpp"
#include "rtos.hpp"
#include "Oled_Display.hpp"
#include "Speaker.hpp"
#include <cstring>

class Hit_Transfer_Control;
class Hit_Run_Control;

// This class handles the game time
class Time_Run_Control : public rtos::task<>
{
public:
	Time_Run_Control(Oled_Display & display, Speaker & speaker):
		task(9, "Time_Run_Control"),
		_clock(this, 1'000'000, "_clock"),
		_time_set_flag(this, "flag"),
		_time_set_pool("pool"),
		_display(display),
		_speaker(speaker)
	{

	}

	void setTime(int time);
	int getTime();


	void giveHitControlPointer(Hit_Run_Control * instance)
	{
		_hitControl = instance;
	}

	void giveTransferControlPointer(Hit_Transfer_Control * instance)
	{
		_transferControl = instance;
	}


protected:
	void main() override;
private:
	int _timeRemaining = -1;

	rtos::clock _clock;
	rtos::flag _time_set_flag;
	rtos::pool< int > _time_set_pool;
	rtos::pool< int > _time_get_pool;

	enum class State { IDLE, ACTIVE, INIT, COUNTDOWN, DONE};
	State _state = State::IDLE;

	Oled_Display & _display;
	Speaker & _speaker;
	Hit_Transfer_Control * _transferControl = nullptr;
	Hit_Run_Control * _hitControl = nullptr;
};

#endif
