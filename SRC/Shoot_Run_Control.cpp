//          Copyright Youri de Vor, Erik de Zeeuw, Hugo Cornel, Matthijs Koelewijn 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "Shoot_Run_Control.hpp"
#include "Hit_Run_Control.hpp"
#include "Game_Parameter_Control.hpp"

void Shoot_Run_Control::main()
{
    for(;;)
    {
        switch( _state )
        {
            case State::IDLE:
            {
                hwlib::wait_ms( 60 );
                if(_fireButton.isButtonPressed() && !_buttonPressed)
                {
                    hwlib::cout << "button";
                    _buttonPressed = true;
                    _state = State::BUTTON_PRESSED;
                }

                if(!_fireButton.isButtonPressed())
                {
                   _buttonPressed = false;
                }
                break;
            }

            case State::BUTTON_PRESSED:
                if(!_hit_run_control->shootIsAvailable())
                {
                    hwlib::cout << "BROKEN";
                    _state = State::IDLE;
                    break;
                }

                int playerID = _game_parameter_control->getPlayerID();
                int weaponPower = _game_parameter_control->getWeaponPower();
                decode(playerID, weaponPower);
                _speaker.playShootTone();
                _send_ir_message_control.send_message(_message);
                _state = State::IDLE;
                break;
        }
    }
}