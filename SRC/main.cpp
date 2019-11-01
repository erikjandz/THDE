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

int main()
{
    WDT->WDT_MR = WDT_MR_WDDIS;
    namespace target = hwlib::target;
    hwlib::wait_ms( 1000 );

    // Hardware objects
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
    auto irSender = IR_Sender(sendPin);
    auto irReceiver = IR_Receiver(receivePin);

    // Task objects
    auto display = Oled_Display(displayOLED, textWriter);
    auto time = Time_Run_Control(display);
    auto hit = Hit_Run_Control();
    auto parameter = Game_Parameter_Control();
    auto send = Send_IR_Message_Control(irSender);
    auto receive = Receive_IR_Message_Control(irReceiver, std::array<Receive_IR_Listener&, 2> { hit, parameter });
    auto speaker = Speaker(speakerPin);
    auto shoot = Shoot_Run_Control(hit, parameter, send, fireButton, speaker);
    (void) display;
    (void) time;
    (void) hit;
    (void) parameter;
   	(void) send;
   	(void) speaker;
   	(void) shoot;

    rtos::run();
}
