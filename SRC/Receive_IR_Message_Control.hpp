#pragma once
#include "hwlib.hpp"
#include "rtos.hpp"
#include "IR_Receiver.hpp"
#include <array>

class Receive_IR_Message_Control: public rtos::task<>{
public:
    template<int N>
    Receive_IR_Message_Control(IR_receiver & _IR_Receiver, std::array<Receive_IR_Listener &, N> _IR_Listeners):
        _IR_Receiver( _IR_Receiver ),
        _IR_Listeners( _IR_Listeners)
        {}

    void main()override{
        for(;;){
            std::array<bool, 16> message = _IR_Receiver.receiveMessage();
            for(auto i : _IR_Listeners){
                i.ReceiveMessage(message);
            }
            hwlib::wait_us(100);
        }
    }

    void decode(std::array<bool, 16> array){
        int playerID = 0;
        int weaponPower = 0;
        int index = 1;
        for(int i = 16; i >= 1; i /=2){
            if(array[index]){
                playerID += i;
            }
            index ++;
        }

        for(int i = 16; i >= 1; i /=2){
            if(array[index]){
                weaponPower += i;
            }
            index ++;
        }
        for(int i = 1; i < 6; i ++){
            if(array[10 + i] != (array[i] ^ array[i + 5])){
                return;
            }
        }
        hwlib::cout << playerID << hwlib::endl << weaponPower << hwlib::endl;
    }

private:
    IR_receiver & _IR_Receiver;
    std::array<Receive_IR_Listener &, N>  _IR_Listeners;
};
