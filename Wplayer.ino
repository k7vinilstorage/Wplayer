#include "display.h"
#include "inputs.h"

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, 32, 33, U8X8_PIN_NONE);

IpodData data;
IpodPlayer player;
IpodInputs inputs;
IpodDisplay display(u8g2, player, data);

TaskHandle_t stream;

char input;

int id = 1;

void setup() {
    Serial.begin(115200);
    display.SetupDisplay();
    data.SdInit();
    data.DbInit("/sd/music.db");
    player.SetupDac();
    player.ChangeVol(-35.00);
    inputs.InitInputs();
    display.MenuInput('z');

    xTaskCreatePinnedToCore(stream_task, "stream", 10000, NULL, 1, &stream,0); 
}

void stream_task(void * pvParameters) {
  // Serial.print("Task1 running on core ");
  // Serial.println(xPortGetCoreID());
  
  for(;;){
    if(!player.StreamAudio()) {
        player.Play(data.RequestItem(id, 'P'));
        display.MenuInput('z');
        id++;
    }
  } 
}

void loop() {
    input = inputs.DetecctInput();
    display.MenuInput(input);
}