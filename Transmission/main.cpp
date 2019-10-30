#include "hwlib.hpp"
#include "transmitter.hpp"
#include "receiver.hpp"
#include <array>

/*void transmit(){
  auto ir = hwlib::target::d2_36kHz();
  auto transmitter = Send_IR_Message_Control(ir, 15, 2);
  //transmitter.sendMessage();
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
  for(;;){
      std::array<bool, 16> array = receiver.receiveMessage();
      for(auto i  : array){
          hwlib::cout << i;
      }
      hwlib::cout << hwlib::endl;
      receiver.decode(array);
  }
}*/

int main(){
    //transmit();
    //receive();
    WDT->WDT_MR = WDT_MR_WDDIS;
    namespace target = hwlib::target;
    hwlib::wait_ms( 1000 );

    auto tsop_signal = target::pin_in( target::pins::d8 );
    auto tsop_gnd    = target::pin_out( target::pins::d9 );
    auto tsop_vdd    = target::pin_out( target::pins::d10 );
    tsop_gnd.write( 0 );
    tsop_vdd.write( 1 );
    tsop_gnd.flush();
    tsop_vdd.flush();
    auto button = target::pin_in(target::pins::d3);
    auto ir = hwlib::target::d2_36kHz();

    auto t1 = Send_IR_Message_Control(ir, 27, 31, button);
    auto t2 = Receive_IR_Message_Control( tsop_signal );
    rtos::run();
}
