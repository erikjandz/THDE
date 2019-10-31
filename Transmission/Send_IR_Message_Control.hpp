#pragma once
#include "IR_sender.hpp"
#include "hwlib.hpp"
#include "rtos.hpp"
#include <array>

class Send_IR_Message_Control: public rtos::task<>{
public:
    Send_IR_Message_Control(IR_sender & _IR_sender, int _weaponPower, int _playerID, hwlib::target::pin_in & pinin):
        _IR_sender( _IR_sender ),
        pinin( pinin )
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
            //add the control bits
            for(int i = 1; i < 6; i ++){
              _message[10 + i] = _message[i] ^ _message[i + 5];
            }
        }

    void main()override{
        for(;;){
          pinin.refresh();
          hwlib::wait_us(100);
          if(!pinin.read()){
              //sendStartTime(15);
              sendMessage();
              hwlib::wait_ms( 1000);
              //_IR_sender.write( 1 );
              //hwlib::wait_ms(1000);
              //_IR_sender.write( 0 );
              //hwlib::wait_ms(1000);
          }

        }
    }

    void sendMessage(){
        for(auto i : _message){
            _send_bit( i );
        }
        hwlib::wait_ms( 3 );
        for(auto i : _message){
            _send_bit( i );
        }
    }

    //send the start time as the leader
    void sendStartTime( int time ){
        /*std::array<bool, 16> array;
        array[0] = 1;
        for(int i = 1; i < 7; i++){
            array[i] = 0;
        }
        int index = 7;
        for(int i = 8; i >= 1; i /= 2){
            if(time >= i){
                time -= i;
                array[index] = 1;
            }else{
                array[index] = 0;
            }
            index ++;
        }
        for(int i = 1; i < 6; i ++){
            array[10 + i] = array[i] ^ array[i + 5];
        }
        _send_16_bits(array);*/

    }
private:
    IR_sender & _IR_sender;
    std::array<bool, 16> _message;
    hwlib::target::pin_in & pinin;

    void _send_bit(bool bit){
        //if bit is a 1
        if(bit){
          _IR_sender.ledOn();
          hwlib::wait_us( 1600 );
          _IR_sender.ledOff();
          hwlib::wait_us( 800 );
        //if bit is a 0
        }else{
          _IR_sender.ledOn();
          hwlib::wait_us( 800 );
          _IR_sender.ledOff();
          hwlib::wait_us( 1600 );
        }
    }
};
