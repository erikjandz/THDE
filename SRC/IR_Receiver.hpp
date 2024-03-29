//          Copyright Youri de Vor, Erik de Zeeuw, Hugo Cornel, Matthijs Koelewijn 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef IR_RECEIVER_HPP
#define IR_RECEIVER_HPP
#include "hwlib.hpp"

/// \brief
/// Class for receiving IR messages
/// \details
// This class serves as an IR message receiver
class IR_receiver
{
public:
	/// \brief
	/// Constructor
	/// \details
    // Constructor needs the IR receiver pin
    IR_receiver(hwlib::target::pin_in & _pin):
        _pin( _pin )
        {}

	/// \brief
	/// Class for hit entity
	/// \details
    // Function that returns the 16-bit IR message
    std::array< bool, 16> receiveMessage()
    {
        for(;;){
            std::array<bool, 16> message1;
            std::array<bool, 16> message2;
            int current_position = 1;
            //wait for first bit
            for(;;){
                if(_read_bit() == 1){
                    message1[0] = 1;
                    break;
                }
            }
            //keep going for the rest
            for(;;){
                int temp = _read_bit();
                if(temp == -1){
                    return receiveMessage();
                }else{
                    message1[current_position] = temp;
                }
                current_position ++;
                if(current_position == 16){
                    break;
                }
            }
            //wait 3 ms and go on
            hwlib::wait_ms( 3 );
            current_position = 0;
            for(;;){
                int temp = _read_bit();
                if(temp == -1){
                    return receiveMessage();
                }else{
                    message2[current_position] = temp;
                }
                current_position ++;
                if(current_position == 16){
                    break;
                }
            }
            if(message1 == message2){
                return message1;
            }
        }

    }
private:
    hwlib::target::pin_in & _pin;

	/// \brief
	/// Function that waits for initial bit
	/// \details
    // Function that waits for the initial IR bit, start of the 16-bit message
    int _wait_for_start(){
        int pause = 0;
        for(;;){
            _pin.refresh();
            bool a = !_pin.read();
            if(a){
                return 1;
            }
            //timed out
            if(pause == 4000){
                return -1;
            }
            hwlib::wait_us(100);
            pause += 100;
        }
    }

	/// \brief
	/// Function that reads a single bit
	/// \details
    // Read a bit, return 0, 1 if bit is 0, 1 and return -1 if nothing is received
    int _read_bit(){
        bool a;
        bool b;
        bool c;
        //wait for the first bit and return is timed out
        if(_wait_for_start() == -1){
            return -1;
        }
        a = 1;
        //then continue the next sequence
        hwlib::wait_us( 800 );
        _pin.refresh();
        b = ! _pin.read();
        hwlib::wait_us( 800 );
        _pin.refresh();
        c = ! _pin.read();
        //hwlib::cout << a << "," << b << "," << c << hwlib::endl;
        if(a && !b && !c){
            return 0;
        }else if(a && b && !c){
            return 1;
        }else{
            return -1;
        }
    }
};

#endif
