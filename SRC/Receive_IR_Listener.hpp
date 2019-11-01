#pragma once
#include "hwlib.hpp"
#include "rtos.hpp"
#include <array>

class Receive_IR_Listener : public rtos::task<>{
public:
    Receive_IR_Listener() : _messageFlag(this, "msgflag")
    {}
    void ReceiveMessage(std::array< bool, 16> message){
        _messagePool.write(message);
        _messageFlag.set();
    }
protected:
    rtos::pool< std::array< bool, 16> > _messagePool;
    rtos::flag _messageFlag;

    void main() override;

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


    int _GetPlayerID(std::array<bool, 16> array){
        int weaponPower = 0;
        int index = 1;
        for(int i = 16; i >= 1; i /=2){
            if(array[index]){
                weaponPower += i;
            }
            index ++;
        }
        return weaponPower;
    }
};
