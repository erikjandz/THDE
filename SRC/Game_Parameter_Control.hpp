#pragma once
#include "hwlib.hpp"
#include "rtos.hpp"
#include "Receive_IR_Listener.hpp"
#include "Time_Run_Control.hpp"

class Game_Parameter_Control: public Receive_IR_Listener{
public:
    Game_Parameter_Control(Time_Run_Control & time_run_control):
        _time_run_control( time_run_control)
    {
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
                    //if you get a message from the leader
                    if(_GetPlayerID(_MessagePool.read()) == 0){
                        _state = State::ACTIVE;
                    }else{
                        _state = State::IDLE;
                    }
                    break;
                case State::ACTIVE:
                    _time_run_control.setTime(_GetWeaponPower(_MessagePool.read()));
                    hwlib::wait_us(60);
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
    Time_Run_Control & _time_run_control;

    enum class State { IDLE, ACTIVE, DECODE };
    State _state = State::IDLE;
};
