//          Copyright Youri de Vor, Erik de Zeeuw, Hugo Cornel, Matthijs Koelewijn 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef HIT_RUN_CONTROL_HPP
#define HIT_RUN_CONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "Receive_IR_Listener.hpp"
#include "ScoreEntity.hpp"

class Oled_Display;
class Speaker;
class Game_Parameter_Control;

// This class serves as a hit receiver. When it receives an IR signal, it lowers the current player's score.
class Hit_Run_Control: public Receive_IR_Listener{
public:
	// The constructor needs pointers to a few classes so they can be accessed in the RTOS main-loop
      Hit_Run_Control(Oled_Display * display, Speaker * speaker, Game_Parameter_Control * parameterControl):
        Receive_IR_Listener(7),
         _displayFlag(this, "displayFlagHit"),
          _display( display ),
          _speaker( speaker ),
          _parameterControl(parameterControl)
          {}

       // This function returns true if the player is allowed to shoot. For example when he is dead, he cannot shoot.
       bool shootIsAvailable();

       // This function returns the current player's score
       int getScore();

       // hier moet ik morgen nog even naar kijken NIET COMMENTEN xx hugo no homo lol hoi joery
       void displayScore();

protected:
	void main() override;

private:
      rtos::pool< int > _score_pool;
      rtos::pool< bool > _shoot_available_pool;
      rtos::flag _displayFlag;
      Oled_Display * _display = nullptr;
      Speaker * _speaker = nullptr;
      Game_Parameter_Control * _parameterControl = nullptr;
      ScoreEntity _score = ScoreEntity(100);

      enum class State { IDLE, UPDATE_SCORE, DISPLAY_SCORE };
      State _state = State::IDLE;
};

#endif
