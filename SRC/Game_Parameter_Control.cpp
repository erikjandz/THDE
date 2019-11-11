//          Copyright Youri de Vor, Erik de Zeeuw, Hugo Cornel, Matthijs Koelewijn 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "Game_Parameter_Control.hpp"
#include "Keypad.hpp"
#include "Oled_Display.hpp"
#include "Time_Run_Control.hpp"
#include "Init_Game_Control.hpp"

void Game_Parameter_Control::main()
{
    char key;
    for(;;){
        switch( _state ){
            case State::IDLE:
                hwlib::wait_ms(60);
                if(_keypad->getKeyPressed() == 'A')
                {
                	hwlib::cout << "A";
                    _state = State::A_PRESSED;
                }
                break;

            case State::A_PRESSED:
                hwlib::wait_ms(60);
                key = _keypad->getKeyPressed();
                if( key >= '0' && key <= '9' ){
                    _playerID = key - 48;
                    hwlib::cout << "number";
                    _state = State::FIRST_NUMBER_ENTERED;
                }
                if(_playerID == 0)
                {
                    _isLeader = true;
                }
                break;

            case State::FIRST_NUMBER_ENTERED:
                hwlib::wait_ms(60);
                if(_keypad->getKeyPressed() == 'B'){
                	hwlib::cout << "B";
                    _state = State::B_PRESSED;
                }
                break;

            case State::B_PRESSED:
                hwlib::wait_ms(60);
                key = _keypad->getKeyPressed();
                if( key >= '0' && key <= '9' ){
                	hwlib::cout << "number";
                    _weaponPower = key - 48;
                    _playerIDPool.write(_playerID);
                    _weaponPowerPool.write(_weaponPower);
                    _state = State::WAITING_FOR_LEADER;
                }
                break;

            case State::WAITING_FOR_LEADER:
                if(_isLeader)
                {
                    _init->setLeader();
                }

                wait( _messageFlag );
               
                if(_GetPlayerID(_messagePool.read()) == 0) // From game leader 
                {
                    _time_run_control->setTime(_GetWeaponPower(_messagePool.read())); // Register game time
                }

                break;
        }
    }
}


int Game_Parameter_Control::getPlayerID(){
    return _playerIDPool.read();
}

int Game_Parameter_Control::getWeaponPower(){
    return _weaponPowerPool.read();
}
