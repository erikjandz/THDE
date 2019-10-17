#pragma once
#include "hwlib.hpp"
#include <array>

class Send_IR_Message_Control{
public:
    Send_IR_Message_Control(hwlib::target::d2_36kHz & _IR_sender, int _weaponPower, int _playerID):
        _IR_sender( _IR_sender )
        {
            //decode the message to a bool array ready to send
            _message[0] = 1;
            int index = 1;
            for(int i = 16; i >= 1; i /= 2){
              if(_playerID >= i){
                _playerID -= i;
                _message[index] = 1;
              }else{
                _message[index] = 0;
              }
              index ++;
            }
            for(int i = 16; i >= 1; i /= 2){
              if(_weaponPower >= i){
                _weaponPower -= i;
                _message[index] = 1;
              }else{
                _message[index] = 0;
              }
              index ++;
            }
            for(int i = 0; i < 5; i ++){
              _message[10 + i] = _message[i] ^ _message[i + 5];
            }
        }
    //send the message twice and wait 3 ms in between
    void sendMessage(){
        for(auto i : _message){
            _send_bit( i );
        }
        hwlib::wait_ms( 3 );
        for(auto i : _message){
            _send_bit( i );
      }
    }
private:
    hwlib::target::d2_36kHz & _IR_sender;
    std::array<bool, 16> _message;
    void _send_bit(bool bit){
        //if bit is a 1
        if(bit){
          _IR_sender.write( 1 );
          hwlib::wait_us( 1600 );
          _IR_sender.write( 0 );
          hwlib::wait_us( 800 );
        //if bit is a 0
        }else{
          _IR_sender.write( 1 );
          hwlib::wait_us( 800 );
          _IR_sender.write( 0 );
          hwlib::wait_us( 1600 );
        }
    }
};
