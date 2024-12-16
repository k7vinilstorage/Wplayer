#include "display.h"

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* clock=*/ 32, /* data=*/ 33, /* reset=*/ U8X8_PIN_NONE);

IpodData data;
IpodPlayer player;
IpodDisplay display(u8g2, data, player);

void setup() {

}

void loop() {
   
}