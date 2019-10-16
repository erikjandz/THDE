#pragma once
#include "hwlib.hpp"
#include <array>

class Transmission{
public:
    Transmission(int value){
        for(int i = 32768; i >= 1; i /= 2){
            if( value >= i){
              
            }
        }
    }
private:
    std::array<bool, 16> _array;
};
