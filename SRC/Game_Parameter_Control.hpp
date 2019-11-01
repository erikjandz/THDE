#pragma once
#include "hwlib.hpp"
#include "rtos.hpp"
#include "Receive_IR_Listener.hpp"

class Game_Parameter_Control: public Receive_IR_Listener{
public:
    Game_Parameter_Control(){
      _playerIDPool.write(5);
      _weaponPowerPool.write(9);
    }

    void main()override{
        for(;;){
            switch( _state ){
                case State::IDLE:
                    wait( _messageFlag );
                    _state = State::DECODE;
                    break;
                case State::DECODE:
                    if(getPlayerID() == 0){
                        _state = State::ACTIVE;
                    }else{
                        _state = State::IDLE;
                    }
                    break;
                case State::ACTIVE:
                    _state = State::IDLE;
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

    enum class State { IDLE, ACTIVE, DECODE };
    State _state = State::IDLE;
};
