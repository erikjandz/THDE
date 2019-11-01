#pragma once
#include "hwlib.hpp"
#include "Beep.hpp"

class Speaker : public rtos::task<> {
public:
  Speaker(hwlib::pin_out& pin): _pin( pin ) {}
  void playEndTone(){ _end_tone_flag.set(); }
  void playShootTone(){ _shoot_tone_flag.set(); }
  void playDeathTone(){ _death_tone_flag.set(); }

  void main() override {
    enum class state_t{IDLE, ENDTONE, SHOOTTONE, DEATHTONE};
    state_t state = IDLE;
    for(;;){
        switch(state){
            case IDLE:
                auto event = wait(_end_tone_flag + _shoot_tone_flag + _death_tone_flag);
                if( event == _end_tone_flag   ){ state = ENDTONE;   }
                if( event == _shoot_tone_flag ){ state = SHOOTTONE; }
                if( event == _death_tone_flag ){ state = DEATHTONE; }
                break;
            case ENDTONE:
                beep::ohhh( _pin );
                state = IDLE;
                break;
            case SHOOTTONE:
                beep::peew( _pin );
                state = IDLE;
                break;
            case DEATHTONE:
                beep::uhoh( _pin );
                state = IDLE;
                break;
        }
    }
  }

private:
  hwlib::target::pin_out& _pin;
  rtos::flag _end_tone_flag;
  rtos::flag _shoot_tone_flag;
  rtos::flag _death_tone_flag;

};
