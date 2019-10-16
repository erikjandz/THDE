#include "hwlib.hpp"
#include "transmitter.hpp"
#include "receiver.hpp"
#include <array>

int main(){
    //receiver
    namespace target = hwlib::target;

    auto tsop_signal = target::pin_in( target::pins::d8 );
    auto tsop_gnd    = target::pin_out( target::pins::d9 );
    auto tsop_vdd    = target::pin_out( target::pins::d10 );
    tsop_gnd.write( 0 );
    tsop_vdd.write( 1 );
    tsop_gnd.flush();
    tsop_vdd.flush();

    auto receiver = Receiver( tsop_signal );
    std::array<bool, 16> array = receiver.receive_message();
    for(auto i  : array){
        hwlib::cout << i << hwlib::endl;
    }
    //Transmitter
    //std::array< bool, 16> message = {1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0};
    //auto ir = target::d2_36kHz();
    //auto transmitter = Transmitter(ir);
    //for(;;){
      //transmitter.send_message(message);
    //}

}
