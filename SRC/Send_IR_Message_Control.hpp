//          Copyright Youri de Vor, Erik de Zeeuw, Hugo Cornel, Matthijs Koelewijn 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef SEND_IR_MESSAGE_CONTROL_HPP
#define SEND_IR_MESSAGE_CONTROL_HPP
#include "IR_Sender.hpp"
#include "hwlib.hpp"
#include "rtos.hpp"
#include <array>

// This class sends IR messages
class Send_IR_Message_Control: public rtos::task<>{
public:
    // Constructor needs IR sender
    Send_IR_Message_Control(IR_sender & _IR_sender):
     task(2, "Send_IR_Message_Control"),
        _IR_sender( _IR_sender ),
        _MessageFlag(this, "msgFlag")
        {}

    // RTOS main
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

    // This function is an interface for other classes to send a message
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

    // This function sends 1 bit based on pauses
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

#endif
