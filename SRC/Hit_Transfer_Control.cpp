#include "Hit_Transfer_Control.hpp"
#include "Hit_Run_Control.hpp"

void Hit_Transfer_Control::main()
{
	for(;;){
	  switch(_state){
	    case State::IDLE:
	      wait(timeReached_flag);
	      _state = State::ACTIVE;
	      break;

	    case State::ACTIVE:
	      int score = _hit_run_control->getScore();
	      hwlib::cout << score;
	      break;
	  }
	}
}

void Hit_Transfer_Control::TimeFlagSet()
{
	timeReached_flag.set();
}