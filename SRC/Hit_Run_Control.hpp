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
#include "HitEntity.hpp"
#include <array>

class Speaker;
class Game_Parameter_Control;

// This class serves as a hit receiver. When it receives an IR signal, it lowers the current player's score.
class Hit_Run_Control: public Receive_IR_Listener
{
public:
      // The constructor needs pointers to a few classes so they can be accessed in the RTOS main-loop
      Hit_Run_Control(Speaker * speaker, Game_Parameter_Control * parameterControl):
      Receive_IR_Listener(7),
          _speaker( speaker ),
          _parameterControl(parameterControl),
          _scoreEntity(100)
       {
          _hitList.fill(HitEntity(-1, -1));
       }

       // This function returns if the player is allowed to shoot. For example when he is dead, he cannot shoot.
       bool shootIsAvailable();

       // This function returns the current player's score
       int getScore();

       // This function returns the list of hits by other players
       std::array<HitEntity, 100> & getHitList();

protected:
  void main() override;

private:
      rtos::pool< int > _score_pool;
      rtos::pool< bool > _shoot_available_pool;
      Speaker * _speaker = nullptr;
      Game_Parameter_Control * _parameterControl = nullptr;
      ScoreEntity _scoreEntity;
      std::array<HitEntity, 100> _hitList;
      int _hitListIndex = 0;
      bool _gameStarted = false;

      enum class State { WAITING };
      State _state = State::WAITING;
};

#endif
