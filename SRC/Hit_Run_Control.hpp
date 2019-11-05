#ifndef HIT_RUN_CONTROL_HPP
#define HIT_RUN_CONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "Receive_IR_Listener.hpp"
#include "ScoreEntity.hpp"

class Oled_Display;
class Speaker;
class Game_Parameter_Control;

class Hit_Run_Control: public Receive_IR_Listener{
public:
      Hit_Run_Control(Oled_Display * display, Speaker * speaker, Game_Parameter_Control * parameterControl):
        Receive_IR_Listener(7),
         _displayFlag(this, "displayFlagHit"),
          _display( display ),
          _speaker( speaker ),
          _parameterControl(parameterControl)
          {}

       bool shootIsAvailable();
       int getScore();
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
