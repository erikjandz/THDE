#pragma once
#include "IR_sender.hpp"
#include "hwlib.hpp"
#include "rtos.hpp"
#include <array>

class Send_IR_Message_Control: public rtos::task<>{
public:
    Send_IR_Message_Control(IR_sender & _IR_sender):
        _IR_sender( _IR_sender )
        {}

    void main()override{
        enum class state_t{IDLE, SEND_MESSAGE};
        state_t state = IDLE;
            for(;;){
                switch( state ):
                    case IDLE:
                        wait( MessageFlag );
                        state = SEND_MESSAGE;
                        break;
                    case SEND_MESSAGE:
                        sendMessage( _MessagePool.read());
                        state = IDLE;
                        break;
              }
    }

    void sendMessage(message){
        for(auto i : message){
            _send_bit( i );
        }
        hwlib::wait_ms( 3 );
        for(auto i : message){
            _send_bit( i );
        }
    }

    void send_message(std::array<bool, 16> message){
        _MessagePool.write(message);
        _MessageFlag.set();

    }

    //send the start time as the leader
    //void sendStartTime( int time ){
        /*std::array<bool, 16> array;
        array[0] = 1;
        for(int i = 1; i < 7; i++){
            array[i] = 0;
        }
        int index = 7;
        for(int i = 8; i >= 1; i /= 2){
            if(time >= i){
                time -= i;
                array[index] = 1;
            }else{
                array[index] = 0;
            }
            index ++;
        }
        for(int i = 1; i < 6; i ++){
            array[10 + i] = array[i] ^ array[i + 5];
        }
        _send_16_bits(array);*/

    //}
private:
    IR_sender & _IR_sender;
    rtos::pool< std::array<bool, 16> > _MessagePool;
    rtos::flag _MessageFlag;

    void _send_bit(bool bit){
        //if bit is a 1
        if(bit){
          _IR_sender.ledOn();
          hwlib::wait_us( 1600 );
          _IR_sender.ledOff();
          hwlib::wait_us( 800 );
        //if bit is a 0
        }else{
          _IR_sender.ledOn();
          hwlib::wait_us( 800 );
          _IR_sender.ledOff();
          hwlib::wait_us( 1600 );
        }
    }
};
