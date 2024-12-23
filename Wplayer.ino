#include "display.h"

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, 32, 33, U8X8_PIN_NONE);

IpodData data;
IpodPlayer player;
IpodDisplay display(u8g2, player, data);

int id = 0;

void setup() {
    Serial.begin(115200);
    display.SetupDisplay();
    player.SetupDac();
    player.ChangeVol(0.00);
    display.MenuInput('z');
    data.DbInit("/sd/music.db");
}

void loop() {
    if(!player.StreamAudio()) {
        player.Play(data.RequestItem(1, 'P'));
        id++;
    }
}