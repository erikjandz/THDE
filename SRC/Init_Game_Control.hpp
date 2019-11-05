#ifndef INIT_GAME_CONTROL_HPP
#define INIT_GAME_CONTROL_HPP
#include "hwlib.hpp"
#include "rtos.hpp"
#include "Keypad.hpp"

class Init_Game_Control : public rtos::task<>{
public:
  Init_Game_Control(Keypad & keypad, Oled_Display & display, Send_IR_Message_Control & send_ir_message_control):
    task(5, "Init_Game_Control"),
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
          hwlib::wait_ms( 60 );
          _display.showText("enter key");
          k = _keyPad.getKeyPressed();
          if(k >= '0' && k <= '9'){
            key = k;
            _display.clear();
            _display.showText(&k);
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
          //_send_ir_message_control.send_message(0, key - 48);
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
};

#endif
