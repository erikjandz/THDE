#include "Hit_Run_Control.hpp"
#include "Speaker.hpp"
#include "Game_Parameter_Control.hpp"

  void Hit_Run_Control::main()
  {
      for(;;){
          switch(_state){
              case State::WAITING:
              {
                _score_pool.write(_scoreEntity.getScore());
                 wait(_messageFlag);

                 if(_gameStarted)
                 {
                    auto msg = _messagePool.read();
                    if(_GetPlayerID(msg) != _parameterControl->getPlayerID() && _scoreEntity.getScore() > 0) // Dont hit yourself
                    {
                      _hitList[_hitListIndex] = HitEntity(_GetPlayerID(msg), _GetWeaponPower(msg));
                      _hitListIndex++;
                      _scoreEntity.setScore(_scoreEntity.getScore() - _GetWeaponPower(msg));
                      _score_pool.write(_scoreEntity.getScore());

                      if(_scoreEntity.getScore() > 0)
                      { // Still alive
                          _shoot_available_pool.write( 1 );
                      }else
                      { // Dead
                      	if(!_isDead)
                      	{
                          _speaker->playDeathTone();
                          _shoot_available_pool.write( 0 );
                          _scoreEntity.setScore(0);
                          _isDead = true;
                      	}
                      }
                    }
                 }
                else
                {
                  _gameStarted = true;
                }

                break;
              }
          }
      }
  }

  bool Hit_Run_Control::shootIsAvailable()
  {
      return _shoot_available_pool.read() && _gameStarted;
  }

  int Hit_Run_Control::getScore()
  {
      return _score_pool.read();
  }

  std::array<HitEntity, 100> & Hit_Run_Control::getHitList()
  {
    return _hitList;
  }
