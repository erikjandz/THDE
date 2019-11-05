#pragma once
#include "hwlib.hpp"
#include "rtos.hpp"
#include "Keypad.hpp"

class Init_Game_Control : public rtos::task<>{
public:
  Init_Game_Control(Keypad & keypad, Oled_Display & display, Send_IR_Message_Control & send_ir_message_control):
    _keyPad(keypad),
    _display(display),
    _send_ir_message_control(send_ir_message_control)
    {}

  void main()override{
    char k;
    char key;
    for(;;){
      switch( _state ){
        case State::IDLE:
          hwlib::wait_ms( 60 );
          k = _keyPad.getKeyPressed();
          if(k == 'C'){
            _state = State::KEYPRESSED;
          }
          break;
        case State::KEYPRESSED:
          hwlib;:wait_ms( 60 );
          _display.showText("enter key");
          k = _keyPad.getKeyPressed();
          if(k >= '0' && k <= '9'){
            key = k;
            _display.clear();
            _display.showText(k);
            _display.showText(" pressed");
            _state = State::NUMBER_ENTERED;
          }
          break;
        case State::NUMBER_ENTERED:
          hwlib::wait_ms( 60 );
          k = _keyPad.getKeyPressed();
          if(k == '#'){
            _state = State::DONE;
          }
          break;
        case State::DONE:
          _send_ir_message_control.send_message(0, key - 48);
          hwlib::wait_ms(1000000);
          break;
      }
    }

  }

private:
  Keypad & _keyPad;
  Oled_Display & _display;
  Send_IR_Message_Control & _send_ir_message_control;

  enum class State { IDLE, KEYPRESSED, NUMBER_ENTERED, DONE};
  State _state = State::IDLE;

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
