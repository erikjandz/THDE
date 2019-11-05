#pragma once
#include "Time_Run_Control.hpp"
#include "hwlib.hpp"
#include "rtos.hpp"
#include "Receive_IR_Listener.hpp"
#include "Keypad.hpp"

class Game_Parameter_Control: public Receive_IR_Listener
{
public:
    Game_Parameter_Control(Keypad & keypad, Oled_Display & display)://, Time_Run_Control & time_run_control):
   		 Receive_IR_Listener(4),
        _keypad( keypad ),
        _display( display )
        //_time_run_control( time_run_control)
    {
      _playerIDPool.write(1);
      _weaponPowerPool.write(0);
    }

    void main()override{
        char key;
        for(;;){
            switch( _state ){
                case State::IDLE:
                    hwlib::wait_ms(60);
                    if(_keypad.getKeyPressed() == 'A')
                    {
                        _state = State::A_PRESSED;
                    }
                    break;
                case State::A_PRESSED:
                    hwlib::wait_ms(60);
                    key = _keypad.getKeyPressed();
                    if( key >= '0' && key <= '9' ){
                        _playerID = key - 48;
                        _state = State::FIRST_NUMBER_ENTERED;
                    }
                    break;
                case State::FIRST_NUMBER_ENTERED:
                    hwlib::wait_ms(60);
                    if(_keypad.getKeyPressed() == 'B'){
                        _state = State::B_PRESSED;
                    }
                    break;
                case State::B_PRESSED:
                    hwlib::wait_ms(60);
                    key = _keypad.getKeyPressed();
                    if( key >= '0' && key <= '9' ){
                        _weaponPower = key - 48;
                        _state = State::SECOND_NUMBER_ENTERED;
                    }
                    break;
                case State::SECOND_NUMBER_ENTERED:
                    _playerIDPool.write(_playerID);
                    _weaponPowerPool.write(_weaponPower);
                    hwlib::cout << _playerID << " " << _weaponPower << hwlib::endl;
                    _state = State::WAITING_FOR_LEADER;
                    break;
                case State::WAITING_FOR_LEADER:
                    wait( _messageFlag );
                    _state = State::IF_LEADER;
                    break;
                case State::IF_LEADER:
                    //if you get a message from the leader
                    if(_GetPlayerID(_messagePool.read()) == 0){
                        _state = State::ACTIVE;
                    }else{
                        _state = State::WAITING_FOR_LEADER;
                    }
                    break;
                case State::ACTIVE:
                    //decode the time and give it to time_run_control
                    //_time_run_control.setTime(_GetWeaponPower(_messagePool.read()));
                    hwlib::wait_ms(10000000);
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
    int _playerID;
    int _weaponPower;
    rtos::pool< int > _playerIDPool;
    rtos::pool< int > _weaponPowerPool;
    Keypad & _keypad;
    Oled_Display & _display;
    //Time_Run_Control & _time_run_control;

    enum class State { IDLE, WAITING_FOR_LEADER, ACTIVE, IF_LEADER, A_PRESSED, B_PRESSED, FIRST_NUMBER_ENTERED, SECOND_NUMBER_ENTERED};
    State _state = State::IDLE;
};
