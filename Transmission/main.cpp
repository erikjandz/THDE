#include "hwlib.hpp"
#include "transmitter.hpp"
#include "receiver.hpp"
#include <array>

void transmit(){
  auto ir = hwlib::target::d2_36kHz();
  auto transmitter = Send_IR_Message_Control(ir, 15, 2);
  transmitter.sendMessage();
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

  auto receiver = Receive_IR_Message_Control( tsop_signal );
  //receiver.print();
  for(;;){
      std::array<bool, 16> array = receiver.receiveMessage();
      for(auto i  : array){
          hwlib::cout << i;
      }
      hwlib::cout << hwlib::endl;
      receiver.decode(array);
  }
}

int main(){
    //transmit();
    receive();
}
