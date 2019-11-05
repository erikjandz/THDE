#ifndef IR_SENDER_HPP
#define IR_SENDER_HPP
#include "hwlib.hpp"
#include "rtos.hpp"

class IR_sender{
public:
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
