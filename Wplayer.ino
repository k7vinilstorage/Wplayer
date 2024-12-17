#include "display.h"
#include <SparkFun_WM8960_Arduino_Library.h> 

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, 32, 33, U8X8_PIN_NONE);

WM8960 teste;

IpodData data;
IpodPlayer player;
IpodDisplay display(u8g2, player, data);

void setup() {}

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