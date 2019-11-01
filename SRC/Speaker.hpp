#pragma once
#include "hwlib.hpp"
#include "beep.hpp"

class Speaker {
public:
  Speaker(hwlib::pin_out& pin): _pin( pin ) {}
  void playEndTone(){
    beep::ohhh( _pin );
  }

  void playShootTone(){
    beep::peew( _pin );
  }

  void playDeathTone(){
    beep::uhoh( _pin );
  }

private:
  hwlib::target::pin_out& _pin;
};
