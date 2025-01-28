#include "display.h"
#include "inputs.h"

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, 32, 33, U8X8_PIN_NONE);

IpodData data;
IpodPlayer player(data);
IpodInputs inputs;
IpodDisplay display(u8g2, player, data);

TaskHandle_t stream;

char input;


void setup() {
    Serial.begin(115200);
    display.SetupDisplay();
    data.SdInit();
    data.DbInit("/sd/music.db");
    player.SetupDac();
    player.vol = data.GetVolume();
    player.ChangeVol();
    player.eq_settings[0] = data.GetBass();
    player.eq_settings[1] = data.GetMiddle();
    player.eq_settings[2] = data.GetTreble();
    player.EQUpdate();
    inputs.InitInputs();
    display.MenuInput('z');

    xTaskCreatePinnedToCore(stream_task, "stream", 10000, NULL, 1, &stream,0); 
}

void stream_task(void * pvParameters) {
  
  for(;;){
    if(player.is_playing) {
      if(!player.StreamAudio()) {
          display.MenuInput('n');
          display.MenuInput('z');
      }
    }
    else {
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
  } 
}

void loop() {
    delay(10);
    input = inputs.DetecctInput();
    display.MenuInput(input);
    //Serial.println(esp_get_free_heap_size());
}