#pragma once
#include "hwlib.hpp"
#include "rtos.hpp"
#include "Receive_IR_Listener.hpp"

class Game_Parameter_Control: public Receive_IR_Listener, public rtos::task<>{
public:
    Game_Parameter_Control(){
      _playerIDPool.write(5);
      _weaponPowerPool.write(9);
    }

    void main()override{
        enum class state_t{IDLE, DECODE, ACTIVE};
        state_t state = IDLE;
        for(;;){
            switch( state ){
                case IDLE:
                    wait( _MessageFlag );
                    state = DECODE;
                    break;
                case DECODE:
                    if(_GetPlayerID == 0){
                        state = ACTIVE;
                    }else{
                        state = IDLE;
                    }
                    break;
                case ACTIVE:
                    state = IDLE;
                    break;
            }
        }
    }

    int getPlayerID(){
        return _playerIDPool.read();
    }

    int getWeaponPower(){
        return _weaponPowerPool.read();
    }
private:
    rtos::pool< int > _playerIDPool;
    rtos::pool< int > _weaponPowerPool;
};
