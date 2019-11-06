//          Copyright Youri de Vor, Erik de Zeeuw, Hugo Cornel, Matthijs Koelewijn 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef GAME_PARAMETER_CONTROL_HPP
#define GAME_PARAMETER_CONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "Receive_IR_Listener.hpp"

class Keypad;
class Oled_Display;
class Time_Run_Control;
class Init_Game_Control;

/// This class is to set game parameters for the player: playerID & weaponPower as well as receiving playtime from the
///
/// This class inherits from the Receive_IR_Listener interface class.
/// It sets game parameters playerID and weaponPower for the player.
/// It also listens to the game leader, who sets the play time.
class Game_Parameter_Control: public Receive_IR_Listener
{
public:
	// The constructor needs pointers to a few classes so they can be accessed in the RTOS main-loop
    Game_Parameter_Control(Keypad * keypad, Oled_Display * display, Time_Run_Control * time_run_control, Init_Game_Control * init):
   		 Receive_IR_Listener(4),
        _keypad( keypad ),
        _display( display ),
        _time_run_control( time_run_control),
        _init(init)
    {
      _playerIDPool.write(1);
      _weaponPowerPool.write(0);
    }

    // This function returns the playerID of this current device
    int getPlayerID();

     // This function returns the weaponPower of this current device
    int getWeaponPower();

protected:
    void main() override;

private:
    int _playerID;
    int _weaponPower;
    rtos::pool< int > _playerIDPool;
    rtos::pool< int > _weaponPowerPool;
    Keypad * _keypad = nullptr;
    Oled_Display * _display = nullptr;
    Time_Run_Control * _time_run_control = nullptr;
    Init_Game_Control * _init = nullptr;

    enum class State { IDLE, WAITING_FOR_LEADER, ACTIVE, IF_LEADER, A_PRESSED, B_PRESSED, FIRST_NUMBER_ENTERED, SECOND_NUMBER_ENTERED};
    State _state = State::IDLE;

    bool _isLeader = false;
};

#endif
