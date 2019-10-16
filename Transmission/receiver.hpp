#pragma once
#include "hwlib.hpp"
#include <array>

class Receiver{
public:
    Receiver(hwlib::target::pin_in & _signal):
        _signal( _signal )
        {}
    std::array< bool, 16> receive_message(){
        std::array<bool, 16> message1;
        std::array<bool, 16> message2;
        int current_position = 0;
        for(;;){
            int temp = _read_bit();
            if(temp == -1){
                temp = _read_bit();
                if(temp == -1){
                    current_position = 0;
                    continue;
                }else{
                    message1[current_position] = temp;
                }
            }else{
                message1[current_position] = temp;
            }
            current_position ++;
            if(current_position == 16){
                break;
            }
        }
        hwlib::wait_ms( 3000 );
        current_position = 0;
        for(;;){
            int temp = _read_bit();
            if(temp == -1){
                temp = _read_bit();
                if(temp == -1){
                    current_position = 0;
                    continue;
                }else{
                    message2[current_position] = temp;
                }
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
        return receive_message();
    }
private:
    hwlib::target::pin_in & _signal;
    //read a bit, return 0, 1 if bit is 0, 1 and return -1 if nothing is received
    int _read_bit(){
        for(;;){
            bool a;
            bool b;
            bool c;
            hwlib::wait_us( 400 );
            _signal.refresh();
            a = ! _signal.read();
            hwlib::wait_us( 800 );
            _signal.refresh();
            b = ! _signal.read();
            hwlib::wait_us( 800 );
            _signal.refresh();
            c = ! _signal.read();
            hwlib::wait_us( 400 );
            if(a && !b && !c){
                return 0;
            }else if(a && b && !c){
                return 1;
            }else{
                return -1;
            }
        }
    }
};
