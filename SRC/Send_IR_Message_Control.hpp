#pragma once
#include "IR_Sender.hpp"
#include "hwlib.hpp"
#include "rtos.hpp"
#include <array>

class Send_IR_Message_Control: public rtos::task<>{
public:
    Send_IR_Message_Control(IR_sender & _IR_sender):
     task(2, "Send_IR_Message_Control"),
        _IR_sender( _IR_sender ),
        _MessageFlag(this, "msgFlag")
        {}

    void main()override
    {
            for(;;)
            {
                switch( _state )
                {
                    case State::IDLE:
                        wait( _MessageFlag );
                        _state = State::SEND_MESSAGE;
                        break;
                    case State::SEND_MESSAGE:
                        auto msg = _MessagePool.read();
                        
                        for(auto i : msg)
                        {
                            _send_bit( i );
                        }
                        hwlib::wait_ms( 3 );
                        for(auto i : msg)
                        {
                            _send_bit( i );
                        }

                        _state = State::IDLE;
                        break;
                 }
            }
    }

    void send_message(std::array<bool, 16> message){
        _MessagePool.write(message);
        _MessageFlag.set();

    }
    
private:
    IR_sender & _IR_sender;
    rtos::pool< std::array<bool, 16> > _MessagePool;
    rtos::flag _MessageFlag;

    enum class State { IDLE, SEND_MESSAGE };
    State _state = State::IDLE;

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
