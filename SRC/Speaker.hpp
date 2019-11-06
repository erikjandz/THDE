//          Copyright Youri de Vor, Erik de Zeeuw, Hugo Cornel, Matthijs Koelewijn 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef SPEAKER_HPP
#define SPEAKER_HPP
#include "hwlib.hpp"
#include "rtos.hpp"

class Speaker : public rtos::task<> {
public:
  Speaker(hwlib::target::pin_out& pin):
  task(0, "Speaker"),
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
              ohhh( _pin );
              state = State::IDLE;
              break;
          }

          case State::SHOOTTONE:
          {
              peew( _pin );
              state = State::IDLE;
              break;
          }

          case State::DEATHTONE:
          {
              uhoh( _pin );
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

  void beep( hwlib::pin_out & lsp, int f, int d, int fd = 1000 ){
     auto t = hwlib::now_us();
     auto end = t + d;
     while( end > hwlib::now_us() ){
         auto p = 500'000 / f;
         f = f * fd / 1000;
         lsp.write( 1 );
         lsp.flush();
         hwlib::wait_us( p );
         lsp.write( 0 );
         lsp.flush();
         hwlib::wait_us( p );
     }
  }

  void peew( hwlib::pin_out & lsp ){
     beep( lsp, 20'000, 200'000, 990 );
  }

  void beep3( hwlib::pin_out & lsp ){
     for( int i = 0; i < 3; i++ ){
         beep( lsp, 1'000, 50'000 );
         hwlib::wait_us( 20'000 );
     }
  }

  void ohhh( hwlib::pin_out & lsp ) {
     for (int i=1000; i<2000; i=i*1.02) {
        beep(lsp,i,10'000);
     }
     for (int i=2000; i>1000; i=i*.98) {
        beep(lsp,i,10'000);
     }
  }

  void uhoh( hwlib::pin_out & lsp ) {
     for (int i=1000; i<1244; i=i*1.01) {
        beep(lsp,i,10'000);
     }
     hwlib::wait_us( 30'000 );
     for (int i=1244; i>1108; i=i*.99) {
        beep(lsp,i,10'000);
     }
  }

  void noise( hwlib::pin_out & lsp ) {
     for (int i = 0; i < 200; i++ ){
        lsp.write( ( hwlib::rand() & 0x01 ) == 0);
        lsp.flush();
        hwlib::wait_us( 1'000 );
     }
  }

  void noise2( hwlib::pin_out & lsp ){
     for (int i = 0; i < 200; i++ ){
        lsp.write( 1 );
        lsp.flush();
        hwlib::wait_us( hwlib::random_in( 500, 2000 ));
        lsp.write( 0 );
        lsp.flush();
        hwlib::wait_us( hwlib::random_in( 500, 2000 ));
     }
  }

  void click( hwlib::pin_out & lsp ){
     for (int i = 0; i < 200; i++ ){
        lsp.write( 1 );
        lsp.flush();
        hwlib::wait_us( hwlib::random_in( 500, 600 ));
        lsp.write( 0 );
        lsp.flush();
        hwlib::wait_us( hwlib::random_in( 500, 600 ));
     }
  }
};

#endif
