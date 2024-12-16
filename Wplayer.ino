#include "display.h"
#include "codec.h"

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* clock=*/ 32, /* data=*/ 33, /* reset=*/ U8X8_PIN_NONE);

IpodDisplay display(u8g2);
IpodCodec dac;

void setup() {
    display.SetupDisplay();
    //dac.StartCodec();
}

void loop() {
    delay(1000);
    display.MenuInput('d');
    delay(1000);
    display.MenuInput('d');
    delay(1000);
    display.MenuInput('d');
    delay(1000);
    display.MenuInput('u');
    delay(1000);
    display.MenuInput('u');
    delay(1000);
    display.MenuInput('u');
    delay(1000);
    display.MenuInput('d');
    delay(1000);
    display.MenuInput('e');
    delay(1000);
    display.MenuInput('d');
    delay(1000);
    display.MenuInput('d');
    delay(1000);
    display.MenuInput('d');
    delay(1000);
    display.MenuInput('u');
    delay(1000);
    display.MenuInput('u');
    delay(1000);
    display.MenuInput('u');
    delay(1000);
    display.MenuInput('b');
    delay(1000);
    display.MenuInput('u');
}