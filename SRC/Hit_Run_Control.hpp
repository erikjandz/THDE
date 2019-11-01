#pragma once
#include "hwlib.hpp"
#include "rtos.hpp"
#include "Receive_IR_Listener.hpp"
#include "ScoreEntity.hpp"
#include "Speaker.hpp"
#include "Oled_Display.hpp"

class Hit_Run_Control: public Receive_IR_Listener{
public:
      Hit_Run_Control(Oled_Display & display, Speaker & speaker):
          _display( display ),
          _speaker( speaker )
          
          {}

      void main() override{
          for(;;){
              switch(_state){
                  case State::IDLE:
                      wait(_messageFlag);
                      _state = State::UPDATE_SCORE;
                      break;
                  case State::UPDATE_SCORE:
                      _score.setScore(_score.getScore() - _GetWeaponPower(_messagePool.read()));
                      _display.showNumber(_score.getScore());
                      _display.showText("Score:");
                      _display.flush();

                      _score_pool.write(_score.getScore());
                      _shoot_available_pool.write(_score.getScore() > 0);

                      if(_score.getScore() > 0){
                          _shoot_available_pool.write( 1 );
                      }else{
                          _speaker.playDeathTone();
                          _shoot_available_pool.write( 0 );
                      }
                      _state = State::IDLE;
                      break;
              }
          }
      }

      bool shootIsAvailable(){
          return _shoot_available_pool.read();
      }

      int getScore(){
          return _score_pool.read();
      }


private:
      rtos::pool< int > _score_pool;
      rtos::pool< bool > _shoot_available_pool;
      Oled_Display & _display;
      Speaker & _speaker;
      ScoreEntity _score = ScoreEntity(100);

      enum class State { IDLE, UPDATE_SCORE };
     State _state = State::IDLE;
};
