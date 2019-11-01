#include "hwlib.hpp"

namespace target = hwlib::target;

class Keypad {
public:
  Keypad( target::pin_oc& _out0,
          target::pin_oc& _out1,
          target::pin_oc& _out2,
          target::pin_oc& _out3,
          target::pin_in& _in0,
          target::pin_in& _in1,
          target::pin_in& _in2,
          target::pin_in& _in3
        ):
          _out0( _out0 ),
          _out1( _out1 ),
          _out2( _out2 ),
          _out3( _out3 ),
          _in0( _in0 ),
          _in1( _in1 ),
          _in2( _in2 ),
          _in3( _in3 )
  {}
  char getKeyPressed(){ return _keyPad.getc(); }
private:
  target::pin_oc& _out0;
  target::pin_oc& _out1;
  target::pin_oc& _out2;
  target::pin_oc& _out3;

  target::pin_in& _in0;
  target::pin_in& _in1;
  target::pin_in& _in2;
  target::pin_in& _in3;

  auto _port_oc = hwlib::port_oc_from( out0, out1, out2, out3    );
  auto _port_in = hwlib::port_in_from( in0,  in1,  in2,  in3     );
  auto _matrix = hwlib::matrix_of_switches( _port_oc, _port_in   );
  auto _keyPad = hwlib::keypad< 16 >( matrix, "123A456B789C*0#D" );
};
