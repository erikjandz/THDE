#pragma once
#include "hwlib.hpp"
#include "rtos.hpp"

class Hit_Run_Control: public rtos::task<>{
public:

private:
      hwlib::glcd_oled & _display;

};
