#pragma once
#include "hwlib.hpp"
#include <array>

class Receiver{
public:
    Receiver(hwlib::target::pin_in & _signal):
        _signal( _signal )
        {}
    std::array< bool, 16> receive_message(){
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
                    return receive_message();
                }else{
                    message1[current_position] = temp;
                }
                current_position ++;
                if(current_position == 16){
                    break;
                }
            }
            //wait a bit and go on
            hwlib::wait_ms( 3 );
            current_position = 0;
            for(;;){
                int temp = _read_bit();
                if(temp == -1){
                    return receive_message();
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
    hwlib::target::pin_in & _signal;

    int _wait_for_start(){
        int pause = 0;
        for(;;){
            _signal.refresh();
            bool a = !_signal.read();
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
    //read a bit, return 0, 1 if bit is 0, 1 and return -1 if nothing is received
    int _read_bit(){
        bool a;
        bool b;
        bool c;
        //wait for the first bit
        if(_wait_for_start() == -1){
            return -1;
        }
        a = 1;
        //then continue the next sequence
        hwlib::wait_us( 900 );
        _signal.refresh();
        b = ! _signal.read();
        hwlib::wait_us( 900 );
        _signal.refresh();
        c = ! _signal.read();
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
