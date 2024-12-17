#include "display.h"

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, 32, 33, U8X8_PIN_NONE);

IpodData data;
IpodPlayer player;
IpodDisplay display(u8g2, player, data);

void setup() {
    Serial.begin(115200);
    display.SetupDisplay();
    player.SetupDac();
    player.ChangeVol(0.00);
    player.Play("/Music/CHOKE.mp3");
    display.MenuInput('z');
}

void loop() {
    // delay(1000);
    // display.MenuInput('d');
    // delay(1000);
    // display.MenuInput('d');
    // delay(1000);
    // display.MenuInput('d');
    // delay(1000);
    // display.MenuInput('u');
    // delay(1000);
    // display.MenuInput('u');
    // delay(1000);
    // display.MenuInput('u');
    // delay(1000);
    // display.MenuInput('d');
    // delay(1000);
    // display.MenuInput('e');
    // delay(1000);
    // display.MenuInput('d');
    // delay(1000);
    // display.MenuInput('d');
    // delay(1000);
    // display.MenuInput('d');
    // delay(1000);
    // display.MenuInput('u');
    // delay(1000);
    // display.MenuInput('u');
    // delay(1000);
    // display.MenuInput('u');
    // delay(1000);
    // display.MenuInput('b');
    // delay(1000);
    // display.MenuInput('u');

    player.StreamAudio();

}