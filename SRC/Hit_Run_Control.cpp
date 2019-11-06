#include "Hit_Run_Control.hpp"
#include "Speaker.hpp"
#include "Game_Parameter_Control.hpp"

  void Hit_Run_Control::main()
  {
      for(;;){
          switch(_state){
              case State::IDLE:
              {
                _score_pool.write(_score.getScore());
                 wait(_messageFlag);
                 if(_gameStarted)
                 {
                  _state = State::UPDATE_SCORE;
                 }
                else
                {
                  _gameStarted = true;
                }
                
                break;
              }

              case State::UPDATE_SCORE:
              {
                  auto msg = _messagePool.read();
                  if(_GetPlayerID(msg) != _parameterControl->getPlayerID()) // Dont hit yourself
                  {
                    _hitList[_hitListIndex] = HitEntity(_GetPlayerID(msg), _GetWeaponPower(msg));
                    _hitListIndex++;
                    _score.setScore(_score.getScore() - _GetWeaponPower(msg));
                    _score_pool.write(_score.getScore());
                    _shoot_available_pool.write(_score.getScore() > 0);

                    if(_score.getScore() > 0){ // Still alive
                        _shoot_available_pool.write( 1 );
                    }else{ // Dead
                        _speaker->playDeathTone();
                        _shoot_available_pool.write( 0 );
                    }
                  }
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

  std::array<HitEntity, 100> & Hit_Run_Control::getHitList()
  {
    return _hitList;
  }