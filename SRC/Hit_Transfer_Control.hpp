#pragma once
#include "hwlib.hpp"
#include "rtos.hpp"
#include "Hit_Run_Control.hpp"

class Hit_Transfer_Control : public rtos::task<>{
public:
  Hit_Transfer_Control(Hit_Run_Control & _hit_run_control):
  _hit_run_control(_hit_run_control), timeReached_flag(this, "flag")
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

  void main()override{
    for(;;){
      switch(_state){
        case State::IDLE:
          wait(timeReached_flag);
          _state = State::ACTIVE;
          break;
        case State::ACTIVE:
          int score = _hit_run_control.getScore();
          hwlib::cout << score;
          break;
      }
    }
  }

  void TimeFlagSet(){
    timeReached_flag.set();
  }

private:
  Hit_Run_Control & _hit_run_control;
  rtos::flag timeReached_flag;
  enum class State{ IDLE, ACTIVE};
  State _state = State::IDLE;
};
