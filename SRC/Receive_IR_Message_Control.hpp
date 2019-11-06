//          Copyright Youri de Vor, Erik de Zeeuw, Hugo Cornel, Matthijs Koelewijn 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef RECEIVE_IR_MESSAGE_CONTROL_HPP
#define RECEIVE_IR_MESSAGE_CONTROL_HPP
#include "hwlib.hpp"
#include "rtos.hpp"
#include "IR_Receiver.hpp"
#include <array>

/// @file
/// \brief
/// Function for signal 1
/// \details
// This class serves as an IR message receiver
template<unsigned int N>
class Receive_IR_Message_Control: public rtos::task<>
{
public:
	// Constructor needs the IR-receiver & IR listeners
    Receive_IR_Message_Control(IR_receiver & _IR_Receiver, std::array<Receive_IR_Listener*, N> _IR_Listeners):
        task(1, "Receive_IR_Message_Control"),
        _IR_Receiver( _IR_Receiver ),
        _IR_Listeners( _IR_Listeners)
        {}

 	// RTOS main
    void main() override
    {
        for(;;)
        {
            std::array<bool, 16> message = _IR_Receiver.receiveMessage();
            for(auto i : _IR_Listeners)
            {
                i->ReceiveMessage(message);
            }
            hwlib::wait_us(100);
        }
    }

    // This function decodes a 16-bit IR message to playerID & weaponPower
    void decode(std::array<bool, 16> array)
    {
        int playerID = 0;
        int weaponPower = 0;
        int index = 1;
        for(int i = 16; i >= 1; i /=2){
            if(array[index]){
                playerID += i;
            }
            index ++;
        }

        for(int i = 16; i >= 1; i /=2){
            if(array[index]){
                weaponPower += i;
            }
            index ++;
        }
        for(int i = 1; i < 6; i ++){
            if(array[10 + i] != (array[i] ^ array[i + 5])){
                return;
            }
        }
    }

private:
    IR_receiver & _IR_Receiver;
    std::array<Receive_IR_Listener*, N>  _IR_Listeners;
};

#endif
