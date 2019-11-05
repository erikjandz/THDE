#ifdef DISPLAY_HPP
#define DISPLAY_HPP

class Oled_Display{
public:
  void showTime(int time){
    d1 << time;
  }
  void showScore(int score){
    d2 << score;
  }

  void showCommand(array<bool, 16> command);
  void clear();

private:
  auto scl = target::pin_oc( target::pins::scl);
  auto sda = target::pin_oc( target::pins::sda);
  auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda(scl, sda);
  auto display = hwlib::glcd_oled(i2c_bus, 0x3c);
  auto w1 = hwlib::part(display, hwlib::xy(0, 0), hwlib::xy(128, 32));
  auto w2 = hwlib::part(display, hwlib::xy(0, 32), hwlib::xy(128, 32));
  auto f1 = hwlib::font_default_16x16();
  auto d1 = hwlib::terminal_from(w1, f1);
  auto f2 = hwlib::font_default_8x8();
  auto d2 = hwlib::terminal_from(w2, f2);
};

#endif //DISPLAY_HPP
