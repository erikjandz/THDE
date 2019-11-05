#ifndef HIT_TRANSFER_CONTROL_HPP
#define HIT_TRANSFER_CONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"

class Hit_Run_Control;

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


    void TimeFlagSet();

protected:
	void main() override;

private:
  Hit_Run_Control * _hit_run_control = nullptr;
  rtos::flag timeReached_flag;
  enum class State{ IDLE, ACTIVE};
  State _state = State::IDLE;
};

#endif
