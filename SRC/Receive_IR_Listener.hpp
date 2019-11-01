#pragma once
#include "hwlib.hpp"
#include "rtos.hpp"
#include <array>

class Receive_IR_Listener{
public:
    void ReceiveMessage(std::array< bool, 16> message){
        MessagePool.write(message);
        MessageFlag.set();
    }
protected:
    rtos::pool< std::array< bool, 16> > _MessagePool;
    rtos::flag _MessageFlag;
};
