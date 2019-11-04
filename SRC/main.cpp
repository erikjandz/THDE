#include "hwlib.hpp"
#include "Time_Run_Control.hpp"
#include "Oled_Display.hpp"
#include "FireButton.hpp"
#include "Shoot_Run_Control.hpp"
#include "Hit_Run_Control.hpp"
#include "Game_Parameter_Control.hpp"
#include "Speaker.hpp"
#include "IR_Sender.hpp"
#include "IR_Receiver.hpp"
#include "Send_IR_Message_Control.hpp"
#include "Receive_IR_Message_Control.hpp"
#include "Keypad.hpp"

int main()
{
    WDT->WDT_MR = WDT_MR_WDDIS;
    namespace target = hwlib::target;
    hwlib::wait_ms( 1000 );

    // Hardware objects
	auto kp0 		 = target::pin_oc( target::pins::a0 );
    auto kp1 		 = target::pin_oc( target::pins::a1 );
    auto kp2 		 = target::pin_oc( target::pins::a2 );
    auto kp3 		 = target::pin_oc( target::pins::a3 );
    auto kp4 		 = target::pin_in( target::pins::a4 );
    auto kp5 		 = target::pin_in( target::pins::a5 );
    auto kp6 		 = target::pin_in( target::pins::a6 );
    auto kp7         = target::pin_in( target::pins::a7 );
    auto kp_out      = hwlib::port_oc_from( kp0, kp1, kp2, kp3 );
    auto kp_in       = hwlib::port_in_from( kp4,  kp5,  kp6,  kp7  );
    auto kp_matrix   = hwlib::matrix_of_switches( kp_out, kp_in );
    auto kp_object   = hwlib::keypad< 16 >( kp_matrix, "123A456B789C*0#D" );
    auto displaySCL  = hwlib::target::pin_oc(hwlib::target::pins::scl);
    auto displaySDA  = hwlib::target::pin_oc(hwlib::target::pins::sda);
    auto displayBUS  = hwlib::i2c_bus_bit_banged_scl_sda(displaySCL, displaySDA);
    auto displayOLED = hwlib::glcd_oled(displayBUS, 0x3c);
    auto font        = hwlib::font_default_16x16();
    auto textWriter  = hwlib::terminal_from(displayOLED, font);
    auto firePin     = hwlib::target::pin_in(hwlib::target::pins::d7);
    auto speakerPin  = hwlib::target::pin_out(hwlib::target::pins::d8);
    auto sendPin     = hwlib::target::d2_36kHz();
	auto receivePin  = target::pin_in( target::pins::d9 );
   	auto receiveGnd  = target::pin_out( target::pins::d10 );
   	auto receiveVCC  = target::pin_out( target::pins::d11 );
   	receiveGnd.write( 0 );
   	receiveVCC.write( 1 );
   	receiveGnd.flush();
   	receiveVCC.flush();

    // Boundary objects
    auto fireButton = FireButton(firePin);
    auto irSender   = IR_sender(sendPin);
    auto irReceiver = IR_receiver(receivePin);
    auto keyPad     = Keypad(kp_object);

    // Task objects
    auto display   = Oled_Display(displayOLED, textWriter);
    auto parameter = Game_Parameter_Control();
    auto send      = Send_IR_Message_Control(irSender);
    auto speaker   = Speaker(speakerPin);
    auto hit       = Hit_Run_Control(display, speaker);
    auto time      = Time_Run_Control(display, hit);
    auto receive   = Receive_IR_Message_Control(irReceiver, std::array<Receive_IR_Listener*, 2> { &hit, &parameter });
    auto shoot     = Shoot_Run_Control(hit, parameter, send, fireButton, speaker);
    (void) display;
    (void) parameter;
   	(void) send;
   	(void) speaker;
   	(void) hit;
   	(void) time;
   	(void) receive;
   	(void) shoot;

    rtos::run();
}
