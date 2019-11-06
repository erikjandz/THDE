#include "Hit_Transfer_Control.hpp"
#include "Hit_Run_Control.hpp"
#include "Time_Run_Control.hpp"

void Hit_Transfer_Control::main()
{
	for(;;){
	  switch(_state){
	    case State::IDLE:
	      wait(_clock);
	      if(_keypad.getKeyPressed() == '*')
	      {
	      	if(_time_run_control->getTime() <= 0)
	      	{
	      		_state = State::ACTIVE;
	      	}
	      }
	      
	      break;

	    case State::ACTIVE:
	  		auto list = _hit_run_control->getHitList();
	  		for(auto & element : list)
	  		{
	  			int playerID = element.getPlayerID();
	  			int weaponPower = element.getWeaponPower();
	  			hwlib::cout << "Player " << playerID << " hit you with weapon power" << weaponPower << hwlib::endl;
	  		}

		_state = State::IDLE;
	      break;
	  }
	}
}