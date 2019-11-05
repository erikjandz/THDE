#pragma once
#include "hwlib.hpp"
#include "rtos.hpp"
#include "Hit_Run_Control.hpp"
#include "Game_Parameter_Control.hpp"
#include "Send_IR_Message_Control.hpp"
#include "FireButton.hpp"

class Shoot_Run_Control: public rtos::task<>{
public:
    Shoot_Run_Control(Hit_Run_Control & hit_run_control, Game_Parameter_Control & game_parameter_control, Send_IR_Message_Control & send_ir_message_control, FireButton & firebutton, Speaker & speaker):
        task(3, "Shoot_Run_Control"),
        _hit_run_control(hit_run_control),
        _game_parameter_control(game_parameter_control),
        _send_ir_message_control(send_ir_message_control),
        _fireButton(firebutton),
        _speaker(speaker)
    {

    }

    void main() override
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
                    if(!_hit_run_control.shootIsAvailable())
                    {
                        hwlib::cout << "BROKEN";
                        _state = State::IDLE;
                        break;
                    }

                    int playerID = _game_parameter_control.getPlayerID();
                    int weaponPower = _game_parameter_control.getWeaponPower();
                   // hwlib::cout << playerID << " " << weaponPower << hwlib::endl;
                    decode(playerID, weaponPower);
                    //_speaker.playShootTone();
                    _send_ir_message_control.send_message(_message);
                    _state = State::IDLE;
                    break;
            }
        }
    }

private:
    Hit_Run_Control & _hit_run_control;
    Game_Parameter_Control & _game_parameter_control;
    Send_IR_Message_Control & _send_ir_message_control;
    FireButton & _fireButton;
    Speaker & _speaker;
    std::array<bool, 16> _message;
    bool _buttonPressed = false;

    enum class State { IDLE, BUTTON_PRESSED };
    State _state = State::IDLE;

    void decode(int playerID, int weaponPower){
        //decode the message to a bool array ready to send
        _message[0] = 1;
        int index = 1;
        for(int i = 16; i >= 1; i /= 2){
          if(playerID >= i){
            playerID -= i;
            _message[index] = 1;
          }else{
            _message[index] = 0;
          }
          index ++;
        }
        for(int i = 16; i >= 1; i /= 2){
          if(weaponPower >= i){
            weaponPower -= i;
            _message[index] = 1;
          }else{
            _message[index] = 0;
          }
          index ++;
        }
        //add the control bits
        for(int i = 1; i < 6; i ++){
          _message[10 + i] = _message[i] ^ _message[i + 5];
        }
    }
};
