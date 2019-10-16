#pragma once
#include "hwlib.hpp"
#include <array>

class Transmitter{
public:
    Transmitter(hwlib::target::d2_36kHz & _signal ):
        _signal( _signal )
        {}
    //send the message twice and wait 3 ms in between
    void send_message(std::array< bool, 16> message){
        for(auto i : message){
            _sent_bit( i );
        }
        hwlib::wait_ms( 3 );
        for(auto i : message){
            _sent_bit( i );
        }
    }
private:
    hwlib::target::d2_36kHz & _signal;
    void _sent_bit(bool bit){
        //if bit is a 0
        if(!bit){
          _signal.write( 1 );
          hwlib::wait_us( 800 );
          _signal.write( 0 );
          hwlib::wait_us( 1600 );
        //if bit is a 1
        }else{
          _signal.write( 1 );
          hwlib::wait_us( 1600 );
          _signal.write( 0 );
          hwlib::wait_us( 800 );
        }
    }
};
