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
    for(;;){
      switch( _state ){
        case State::IDLE:
          hwlib::wait_ms( 60 );
          k = _keyPad.getKeyPressed();
          if(k == 'c'){
            _state = State::KEYPRESSED;
          }
          break;
        case State::KEYPRESSED:
          _display.showText("enter key");
          k = _keyPad.getKeyPressed();
          if(k >= '0' && k <= '9'){
            _display.clear();
            _display.showText(&k);
            _display.showText(" pressed");
          }
      }
    }

  }

private:
  Keypad & _keyPad;
  Oled_Display & _display;
  Send_IR_Message_Control & _send_ir_message_control;

  enum class State { IDLE, KEYPRESSED, };
  State _state = State::IDLE;
};
