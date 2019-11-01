#pragma once
#include "hwlib.hpp"
#include "rtos.hpp"

class Game_Parameter_Control: public Receive_IR_Listener, public rtos::task<>{
public:
    Game_Parameter_Control(){
      _playerIDPool.write(5);
    }

    void main()override{
        enum class state_t{IDLE, DECODE, ACTIVE};

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
