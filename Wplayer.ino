#include "display.h"

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, 32, 33, U8X8_PIN_NONE);

IpodData data;
IpodPlayer player;
IpodDisplay display(u8g2, player, data);

int id = 1;

void setup() {
    Serial.begin(115200);
    data.SdInit();
    data.DbInit("/sd/music.db");
    display.SetupDisplay();
    player.SetupDac();
    player.ChangeVol(-35.00);
    display.MenuInput('z');
}

void loop() {
    if(!player.StreamAudio()) {
        player.Play(data.RequestItem(id, 'P'));
        display.MenuInput('z');
        id++;
    }
}