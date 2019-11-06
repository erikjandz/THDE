//          Copyright Youri de Vor, Erik de Zeeuw, Hugo Cornel, Matthijs Koelewijn 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

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
	      		auto list = _hit_run_control->getHitList(); // Get list of hits

		  		for(auto & element : list) // Loop through hits
		  		{
		  			if(element.getPlayerID() != -1) // If not uninitialized
		  			{
			  			int playerID = element.getPlayerID();
			  			int weaponPower = element.getWeaponPower();
			  			hwlib::cout << "Player " << playerID << " hit you with weapon power" << weaponPower << hwlib::endl;
		  			}
		  		}
	      	}
	      }
	      
	      break;
	  }
	}
}