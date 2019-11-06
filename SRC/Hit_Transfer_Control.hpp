//          Copyright Youri de Vor, Erik de Zeeuw, Hugo Cornel, Matthijs Koelewijn 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef HIT_TRANSFER_CONTROL_HPP
#define HIT_TRANSFER_CONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"

class Hit_Run_Control;


/// Used to output the log of hits taken by the player.
///
/// At the end of the game, all players need to output who they got shot by and
/// how many times they got shot by the other players. This is done by keeping a log of
/// all recorded hits which is then output to a terminal session at the end of the game.
class Hit_Transfer_Control : public rtos::task<>{
public:
  Hit_Transfer_Control(Hit_Run_Control * hit_run_control):
    task(6, "Hit_Transfer_Control"),
    _hit_run_control(hit_run_control),
    timeReached_flag(this, "flag")
  {}
  //
  // playerScore(int playerIndex){
  //   hwlib::cout << score[playerIndex];
  // }
  //
  // printScore(){
  //   for(int i = 0; i < numPlayers; i++){
  //     hwlib::cout << score[i];
  //   }
  // }

  /// public method to set the timeReached_flag
  ///
  /// This method serves as an interface for other classes to tell a Hit_Transfer_Control object that game has ended
  void TimeFlagSet();

protected:
  // please fix me
	void main() override;

private:
  Hit_Run_Control * _hit_run_control = nullptr;
  rtos::flag timeReached_flag;
  enum class State{ IDLE, ACTIVE};
  State _state = State::IDLE;
};

#endif
