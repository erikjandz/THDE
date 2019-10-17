#include "hwlib.hpp"
#include "transmitter.hpp"
#include "receiver.hpp"
#include <array>

void transmit(){
  std::array< bool, 16> message = {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0};
  auto ir = hwlib::target::d2_36kHz();
  auto transmitter = Transmitter(ir);
  //for(;;){
    transmitter.send_message(message);
  //}
  //transmitter.debug();
}

void receive(){
  namespace target = hwlib::target;

  auto tsop_signal = target::pin_in( target::pins::d8 );
  auto tsop_gnd    = target::pin_out( target::pins::d9 );
  auto tsop_vdd    = target::pin_out( target::pins::d10 );
  tsop_gnd.write( 0 );
  tsop_vdd.write( 1 );
  tsop_gnd.flush();
  tsop_vdd.flush();

  auto receiver = Receiver( tsop_signal );
  //receiver.print();
  for(;;){
      std::array<bool, 16> array = receiver.receive_message();
      for(auto i  : array){
          hwlib::cout << i;
      }
      hwlib::cout << hwlib::endl;
  }
}

int main(){
    //transmit();
    receive();
}
