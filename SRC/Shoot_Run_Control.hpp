#pragma once
#include "hwlib.hpp"
#include "rtos.hpp"
#include "Hit_Run_Control.hpp"
#include "Game_Parameter_Control.hpp"
#include "Send_IR_Message_Control.hpp"
#include "FireButton.hpp"

class Shoot_Run_Control: public rtos::task<>{
public:
    Shoot_Run_Control(Hit_Run_Control & _hit_run_control, Game_Parameter_Control & _game_parameter_control, Send_IR_Message_Control & _send_ir_message_control, FireButton & _firebutton, Speaker & _speaker):
        _hit_run_control(_hit_run_control),
        _game_parameter_control(_game_parameter_control),
        _send_ir_message_control(_send_ir_message_control),
        _firebutton(_firebutton),
        _speaker(_speaker )
        {}

    void main()override{
        enum class state_t{IDLE, BUTTONPRESSED};
        state_t state = IDLE;
        for(;;){
            switch( case ){
                case IDLE:
                    wait_ms( 60 );
                    if(_fireButton.isButtonPressed()){
                        state = BUTTONPRESSED;
                    }
                    break;
                case BUTTONPRESSED:
                    if(!_hit_run_control.shootIsAvailable()){
                        state = IDLE;
                        break;
                    }
                    playerID = _game_parameter_control.getPlayerID;
                    weaponPower = _game_parameter_control.getWeaponPower;
                    _speaker.playShootTone;
                    _send_ir_message_control.send_message(_message);
                    state = IDLE;
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
    int _playerID;
    int weaponPower;
    std::array<bool, 16> & _message;

    void decode(int playerID, int weaponPower){
        //decode the message to a bool array ready to send
        _message[0] = 1;
        int index = 1;
        for(int i = 16; i >= 1; i /= 2){
          if(_playerID >= i){
            _playerID -= i;
            _message[index] = 1;
          }else{
            _message[index] = 0;
          }
          index ++;
        }
        for(int i = 16; i >= 1; i /= 2){
          if(_weaponPower >= i){
            _weaponPower -= i;
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
