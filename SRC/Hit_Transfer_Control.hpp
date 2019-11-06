//          Copyright Youri de Vor, Erik de Zeeuw, Hugo Cornel, Matthijs Koelewijn 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef HIT_TRANSFER_CONTROL_HPP
#define HIT_TRANSFER_CONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "Keypad.hpp"

class Hit_Run_Control;
class Time_Run_Control;

class Hit_Transfer_Control : public rtos::task<>{
public:
  Hit_Transfer_Control(Hit_Run_Control * hit_run_control, Time_Run_Control * time_run_control, Keypad & keypad):
    task(6, "Hit_Transfer_Control"),
    _hit_run_control(hit_run_control),
    _time_run_control(time_run_control),
    _clock(this, 60'000, "_clockkkkk"),
    _keypad(keypad)
	{

	}

protected:
	void main() override;

private:
  Hit_Run_Control * _hit_run_control = nullptr;
  Time_Run_Control * _time_run_control = nullptr;
  rtos::clock _clock;
  enum class State{ IDLE, ACTIVE};
  State _state = State::IDLE;
  Keypad & _keypad;
};

#endif
