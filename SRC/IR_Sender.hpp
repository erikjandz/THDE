//          Copyright Youri de Vor, Erik de Zeeuw, Hugo Cornel, Matthijs Koelewijn 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef IR_SENDER_HPP
#define IR_SENDER_HPP
#include "hwlib.hpp"
#include "rtos.hpp"

// This class sends IR bits through an IR sender pin
class IR_sender
{
public:
    // Constructor needs IR sender pin
    IR_sender(hwlib::target::d2_36kHz & _pin):
        _pin( _pin )
        {}
    void ledOn(){
        _pin.write( 1 );
    }

    void ledOff(){
        _pin.write( 0 );
    }
private:
    hwlib::target::d2_36kHz & _pin;
};

#endif
