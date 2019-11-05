#pragma once
#include "hwlib.hpp"
#include "rtos.hpp"
#include "Receive_IR_Listener.hpp"
#include "ScoreEntity.hpp"
#include "Speaker.hpp"
#include "Oled_Display.hpp"
#include "Game_Parameter_Control.hpp"

class Hit_Run_Control: public Receive_IR_Listener{
public:
      Hit_Run_Control(Oled_Display & display, Speaker & speaker, Game_Parameter_Control & parameterControl):
        Receive_IR_Listener(7),
         _displayFlag(this, "displayFlagHit"),
          _display( display ),
          _speaker( speaker ),
          _parameterControl(parameterControl)
          {}

      void main() override{
          for(;;){
              switch(_state){
                  case State::IDLE:
                  {
                      _score_pool.write(_score.getScore());
                      auto evt = wait(_messageFlag + _displayFlag);
                      if(evt == _messageFlag)
                      {
                          hwlib::cout << "debug";
                          _state = State::UPDATE_SCORE;
                      }
                      else if(evt == _displayFlag)
                      {
                        _state = State::DISPLAY_SCORE;
                      }
                      
                      break;
                  }

                  case State::UPDATE_SCORE:
                  {
                      auto msg = _messagePool.read();
                      if(_GetPlayerID(msg) != _parameterControl.getPlayerID())
                      {
                        _score.setScore(_score.getScore() - _GetWeaponPower(msg));
                        _score_pool.write(_score.getScore());
                        _shoot_available_pool.write(_score.getScore() > 0);

                        if(_score.getScore() > 0){
                            _shoot_available_pool.write( 1 );
                        }else{
                            _speaker.playDeathTone();
                            _shoot_available_pool.write( 0 );
                        }
                      }
                      _state = State::IDLE;
                      break;
                  }

                  case State::DISPLAY_SCORE:
                  {
                      _display.showText("Score:");
                      _display.newLine();
                      _display.showNumber(_score.getScore());
                      _display.flush();
                      _state = State::IDLE;
                      break;
                  }
              }
          }
      }

      bool shootIsAvailable()
      {
          return _shoot_available_pool.read();
      }

      int getScore()
      {
          return _score_pool.read();
      }

      void displayScore()
      {
         _displayFlag.set();
      }


private:
      rtos::pool< int > _score_pool;
      rtos::pool< bool > _shoot_available_pool;
      rtos::flag _displayFlag;
      Oled_Display & _display;
      Speaker & _speaker;
      Game_Parameter_Control & _parameterControl;
      ScoreEntity _score = ScoreEntity(100);

      enum class State { IDLE, UPDATE_SCORE, DISPLAY_SCORE };
      State _state = State::IDLE;
};
