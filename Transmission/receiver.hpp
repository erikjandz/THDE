#pragma once
#include "hwlib.hpp"
#include <array>

class Receive_IR_Message_Control{
public:
    Receive_IR_Message_Control(hwlib::target::pin_in & _IR_Receiver):
        _IR_Receiver( _IR_Receiver )
        {}
    std::array< bool, 16> receiveMessage(){
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

    void decode(std::array<bool, 16> array){
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
        for(int i = 0; i < 5; i ++){
            if(array[10 + i] != (array[i] ^ array[i + 5])){
                return;
            }
        }
        hwlib::cout << playerID << hwlib::endl << weaponPower << hwlib::endl;
    }

private:
    hwlib::target::pin_in & _IR_Receiver;

    //wait for the first signal to arrive
    int _wait_for_start(){
        int pause = 0;
        for(;;){
            _IR_Receiver.refresh();
            bool a = !_IR_Receiver.read();
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
        _IR_Receiver.refresh();
        b = ! _IR_Receiver.read();
        hwlib::wait_us( 900 );
        _IR_Receiver.refresh();
        c = ! _IR_Receiver.read();
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
