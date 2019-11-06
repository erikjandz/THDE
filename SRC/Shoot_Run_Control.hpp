//          Copyright Youri de Vor, Erik de Zeeuw, Hugo Cornel, Matthijs Koelewijn 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef SHOOT_RUN_CONTROL_HPP
#define SHOOT_RUN_CONTROL_HPP
#include "hwlib.hpp"
#include "rtos.hpp"
#include "FireButton.hpp"
#include "Send_IR_Message_Control.hpp"
#include "Speaker.hpp"

class Hit_Run_Control;
class Game_Parameter_Control;

class Shoot_Run_Control: public rtos::task<>{
public:
    Shoot_Run_Control(Hit_Run_Control * hit_run_control, Game_Parameter_Control * game_parameter_control, Send_IR_Message_Control & send_ir_message_control, FireButton & firebutton, Speaker & speaker):
        task(3, "Shoot_Run_Control"),
        _hit_run_control(hit_run_control),
        _game_parameter_control(game_parameter_control),
        _send_ir_message_control(send_ir_message_control),
        _fireButton(firebutton),
        _speaker(speaker)
    {

    }

protected:
	void main() override;

private:
    Hit_Run_Control * _hit_run_control;
    Game_Parameter_Control * _game_parameter_control;
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

#endif
