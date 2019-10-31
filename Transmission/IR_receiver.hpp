#pragma once
#include "hwlib.hpp"
#include "rtos.hpp"

class IR_receiver{
public:
    IR_receiver(hwlib::target::pin_in & _pin):
        _pin( _pin )
        {}
private:
    hwlib::target::pin_in & _pin;
};

#pragma once
#include "hwlib.hpp"
#include "rtos.hpp"
