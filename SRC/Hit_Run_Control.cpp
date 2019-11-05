#include "Hit_Run_Control.hpp"
#include "Speaker.hpp"
#include "Oled_Display.hpp"
#include "Game_Parameter_Control.hpp"

  void Hit_Run_Control::main()
  {
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
                  if(_GetPlayerID(msg) != _parameterControl->getPlayerID())
                  {
                    _score.setScore(_score.getScore() - _GetWeaponPower(msg));
                    _score_pool.write(_score.getScore());
                    _shoot_available_pool.write(_score.getScore() > 0);

                    if(_score.getScore() > 0){
                        _shoot_available_pool.write( 1 );
                    }else{
                        _speaker->playDeathTone();
                        _shoot_available_pool.write( 0 );
                    }
                  }
                  _state = State::IDLE;
                  break;
              }

              case State::DISPLAY_SCORE:
              {
                  _display->showText("Score:");
                  _display->newLine();
                  _display->showNumber(_score.getScore());
                  _display->flush();
                  _state = State::IDLE;
                  break;
              }
          }
      }
  }

  bool Hit_Run_Control::shootIsAvailable()
  {
      return _shoot_available_pool.read();
  }

  int Hit_Run_Control::getScore()
  {
      return _score_pool.read();
  }

  void Hit_Run_Control::displayScore()
  {
     _displayFlag.set();
  }
