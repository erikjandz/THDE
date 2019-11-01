#pragma once
#include "hwlib.hpp"
#include "rtos.hpp"
#include "Receive_IR_Listener.hpp"
#include "ScoreEntity.hpp"

class Hit_Run_Control: public Receive_IR_Listener, public rtos::task<>{
public:
      Hit_Run_Control(hwlib::glcd_oled & _display, Speaker & _speaker):
          _display( _display ),
          _speaker( _speaker )
          {}

      void main()override{
          enum class state_t{IDLE, UPDATE_SCORE};
          state_t state = IDLE;
          for(;;){
              switch(state){
                  case IDLE:
                      wait(_MessageFlag);
                      state = UPDATE_SCORE;
                      break;
                  case UPDATE_SCORE:
                      _score.setScore(_score.getScore() - _GetWeaponPower(_MessagePool.read()));
                      _display.showMessage(_score_pool.read());
                      if(_score.getScore() > 0){
                          _shoot_available_pool.write( 1 );
                      }else{
                          _speaker.playDeathTone(_score.getScore());
                          _shoot_available_pool.write( 0 );
                      }
                      state = IDLE;
                      break;
              }
          }
      }

      bool shootIsAvailable(){
          return _shoot_available_pool.read();
      }

      int getScore(){
          return _score_pool.read();
      }


private:
      rtos::pool< int > _score_pool;
      rtos::pool< bool > _shoot_available_pool;
      hwlib::glcd_oled & _display;
      Speaker & _speaker;
      ScoreEntity _score(100);

      int _GetWeaponPower(std::array<bool, 16> array){
          int weaponPower = 0;
          int index = 6;
          for(int i = 16; i >= 1; i /=2){
              if(array[index]){
                  weaponPower += i;
              }
              index ++;
          }
          return weaponPower;
      }

};
