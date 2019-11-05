#ifndef SPEAKER_HPP
#define SPEAKER_HPP
#include "hwlib.hpp"
#include "Beep.hpp"
#include "rtos.hpp"

class Speaker : public rtos::task<> {
public:
  Speaker(hwlib::target::pin_out& pin):
  task(10, "Speaker"),
  _pin( pin ),
   _end_tone_flag(this, "end"),
   _shoot_tone_flag(this, "shoot"),
   _death_tone_flag(this, "death")
   {}
  void playEndTone(){ _end_tone_flag.set(); }
  void playShootTone(){ _shoot_tone_flag.set(); }
  void playDeathTone(){ _death_tone_flag.set(); }

  void main() override
   {
    for(;;)
    {
        switch(state)
        {
          case State::IDLE:
          {
              auto event = wait(_end_tone_flag + _shoot_tone_flag + _death_tone_flag);
              if( event == _end_tone_flag   ){ state = State::ENDTONE;   }
              if( event == _shoot_tone_flag ){ state = State::SHOOTTONE; }
              if( event == _death_tone_flag ){ state = State::DEATHTONE; }
              break;
          }

          case State::ENDTONE:
          {
              beep::ohhh( _pin );
              state = State::IDLE;
              break;
          }

          case State::SHOOTTONE:
          {
              beep::peew( _pin );
              state = State::IDLE;
              break;
          }

          case State::DEATHTONE:
          {
              beep::uhoh( _pin );
              state = State::IDLE;
              break;
          }
        }
    }
  }

private:
  hwlib::target::pin_out& _pin;
  rtos::flag _end_tone_flag;
  rtos::flag _shoot_tone_flag;
  rtos::flag _death_tone_flag;

  enum class State { IDLE, ENDTONE, SHOOTTONE, DEATHTONE };
  State state = State::IDLE;
};

#endif
