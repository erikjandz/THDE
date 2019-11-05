#include "Game_Parameter_Control.hpp"
#include "Keypad.hpp"
#include "Oled_Display.hpp"
#include "Time_Run_Control.hpp"
#include "Init_Game_Control.hpp"

void Game_Parameter_Control::main()
{
    char key;
    for(;;){
        switch( _state ){
            case State::IDLE:
                hwlib::wait_ms(60);
                if(_keypad->getKeyPressed() == 'A')
                {
                    _state = State::A_PRESSED;
                }
                break;

            case State::A_PRESSED:
                hwlib::wait_ms(60);
                key = _keypad->getKeyPressed();
                if( key >= '0' && key <= '9' ){
                    _playerID = key - 48;
                    _state = State::FIRST_NUMBER_ENTERED;
                }
                if(_playerID == 0)
                {
                    _isLeader = true;
                }
                break;

            case State::FIRST_NUMBER_ENTERED:
                hwlib::wait_ms(60);
                if(_keypad->getKeyPressed() == 'B'){
                    _state = State::B_PRESSED;
                }
                break;

            case State::B_PRESSED:
                hwlib::wait_ms(60);
                key = _keypad->getKeyPressed();
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
                if(_isLeader)
                {
                    hwlib::cout << "kip123";
                    _init->setLeader();
                }
                wait( _messageFlag );
                _state = State::IF_LEADER;
                break;

            case State::IF_LEADER:
                hwlib::cout << "hoi";
                //if you get a message from the leader
                if(_GetPlayerID(_messagePool.read()) == 0){
                    _state = State::ACTIVE;
                }else{
                    _state = State::WAITING_FOR_LEADER;
                }
                break;
            case State::ACTIVE:
                //decode the time and give it to time_run_control
                hwlib::cout << "dit is de hemel";
                _time_run_control->setTime(_GetWeaponPower(_messagePool.read()));
                hwlib::wait_ms(100000000);
                break;
        }
    }
}


int Game_Parameter_Control::getPlayerID(){
    return _playerIDPool.read();
}

int Game_Parameter_Control::getWeaponPower(){
    return _weaponPowerPool.read();
}