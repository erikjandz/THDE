//          Copyright Youri de Vor, Erik de Zeeuw, Hugo Cornel, Matthijs Koelewijn 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef RECEIVE_IR_LISTENER_HPP
#define RECEIVE_IR_LISTENER_HPP
#include "hwlib.hpp"
#include "rtos.hpp"
#include <array>

/// \brief
/// Class for receiving IR messages
/// \details
// This class is a listener from which classes can inherit from. 
// It is also a task because it can receive IR messages.
class Receive_IR_Listener : public rtos::task<>
{
public:
	/// \brief
	/// Constructor
	/// \details
	// Custom priority needed by constructor
    Receive_IR_Listener(unsigned int priority) :
        task(priority, "Receive_IR_Listener"),
    _messageFlag(this, "msgflag")
    {

    }

	/// \brief
	/// Function for receiving IR messages
	/// \details
    // This function receives the 16-bit IR message
    void ReceiveMessage(std::array< bool, 16> message)
    {
        _messagePool.write(message);
        _messageFlag.set();
    }
protected:
    rtos::pool< std::array< bool, 16> > _messagePool;
    rtos::flag _messageFlag;

	/// \brief
	/// Main function
	/// \details
    // RTOS main
    void main() override 
    {

    }

	/// \brief
	/// Function that decodes IR
	/// \details
    // This function decodes the weapon power value from the bits
    int _GetWeaponPower(std::array<bool, 16> array){
        int weaponPower = 0;
        int index = 6;
        for(int i = 16; i >= 1; i /=2){
            if(array[index]){
                weaponPower += i;
            }
            index ++;
        }
        return weaponPower;
    }

	/// \brief
	/// Function that decodes IR
	/// \details
    // This function decodes the player ID value from the bits
    int _GetPlayerID(std::array<bool, 16> array){
        int weaponPower = 0;
        int index = 1;
        for(int i = 16; i >= 1; i /=2){
            if(array[index]){
                weaponPower += i;
            }
            index ++;
        }
        return weaponPower;
    }
};

#endif
