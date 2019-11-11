//          Copyright Youri de Vor, Erik de Zeeuw, Hugo Cornel, Matthijs Koelewijn 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef INIT_GAME_CONTROL_HPP
#define INIT_GAME_CONTROL_HPP
#include "hwlib.hpp"
#include "rtos.hpp"
#include "Keypad.hpp"
#include "Send_IR_Message_Control.hpp"

/// \brief
/// Class for init game
/// \details
/// Used to send game parameters to other players, but mainly meant to start the game at the same time for all players.
/// This class takes input from the user, through the keypad, and adds these inputs to an array.
/// These inputs represent the game time.
/// This array _message is then sent to other players.
class Init_Game_Control : public rtos::task<>{
public:
  Init_Game_Control(Keypad & keypad, Oled_Display & display, Send_IR_Message_Control & send_ir_message_control):
    task(5, "Init_Game_Control"),
    _keyPad(keypad),
    _display(display),
    _send_ir_message_control(send_ir_message_control),
    _leaderFlag(this, "_leaderFlag"),
    _clock(this, 60'000, "clockkk")
    {}

	/// \brief
	/// Function that sets current player to game leader
	/// \details
	/// public method to set the _leaderFlag
	/// serves as an interface for other classes to tell Init that this arduino is the game leader.
	void setLeader()
	{
	  _leaderFlag.set();
	}

  /// \brief
  /// Main function
  /// \details
  /// the main of this class
  /// takes care of the task switching and calling the necessary methods to receive input from the keyboard and send messages using the IR transmitter
  void main()override{
    char k;
    char key;
    for(;;){
      switch( _state ){
        case State::BEGIN:
          wait(_leaderFlag);
          _state = State::IDLE;
          break;

        case State::IDLE:
          wait( _clock);
          k = _keyPad.getKeyPressed();
          if(k == 'C'){
            hwlib::cout << "C";
            _state = State::KEYPRESSED;
          }
          break;
        case State::KEYPRESSED:
          hwlib::wait_ms( 60 );
          _display.clear();
          _display.showText("enter key");
          _display.flush();
          k = _keyPad.getKeyPressed();
          if(k >= '0' && k <= '9'){
            hwlib::cout << "time";
            key = k;
            _display.clear();
            _display.showText(&k);
            _display.showText(" pressed");
            _display.flush();
            _state = State::NUMBER_ENTERED;
          }
          break;
        case State::NUMBER_ENTERED:
          hwlib::wait_ms( 60 );
          k = _keyPad.getKeyPressed();
          if(k == '#'){
            hwlib::cout << "#";
            _state = State::DONE;
          }
          break;
        case State::DONE:
          decode(0, key-48);
          _send_ir_message_control.send_message(_message);
          hwlib::wait_ms(100'000'000);
          break;
      }
    }

  }

private:
  Keypad & _keyPad;
  Oled_Display & _display;
  Send_IR_Message_Control & _send_ir_message_control;
  rtos::flag _leaderFlag;
  rtos::clock _clock;
  enum class State { IDLE, KEYPRESSED, NUMBER_ENTERED, DONE, BEGIN};
  State _state = State::BEGIN;
  std::array<bool, 16> _message;

	/// \brief
	/// Function that decodes to playerID and gameTime
	/// \details
	void decode(int playerID, int gameTime)
	{
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
	    if(gameTime >= i){
	      gameTime -= i;
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
