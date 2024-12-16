#include <U8g2lib.h>
#include <Wire.h>
#include "images.h"

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* clock=*/ 32, /* data=*/ 33, /* reset=*/ U8X8_PIN_NONE);

bool sd_state = false; //to the data library

bool is_playing = false; //to player library

int main_menu_pos = 0; //UI
int settings_menu_pos = 0; //UI
int selected_menu = 0; //UI



void SetupDisplay() {
    Wire.begin(33,32);
    u8g2.begin();
    u8g2.clearBuffer();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.drawXBMP(1, 0, 127, 64, ipod_w);
    u8g2.sendBuffer();
    delay(2000);
    MainMenu('z');
}

void SdDraw() {
    if(sd_state) {
        u8g2.drawXBMP(112, 46, 12, 15, image_micro_sd_bits);
    }
    else {
        u8g2.drawXBMP(111, 46, 14, 15, image_micro_sd_no_card_bits);
    }
}

void PlayingIndicator() {
    if(is_playing) {
        u8g2.drawXBMP(103, 23, 15, 15, image_music_play_bits);
    }
    else {
        u8g2.drawXBMP(103, 23, 15, 15, image_music_pause_bits);
    }
}

void SidebarDraw() {
    SdDraw();
    PlayingIndicator();
    u8g2.drawXBMP(94, 46, 14, 15, image_bluetooth_not_connected_bits);
    u8g2.drawXBMP(97, 3, 24, 13, image_battery_empty_bits);
    u8g2.drawLine(89, 0, 89, 63);
    u8g2.drawLine(90, 64, 90, 0);
}

void MainMenuDraw() {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tr);
    SidebarDraw();

    switch(main_menu_pos) {
        case 0:
            u8g2.drawXBMP(0, 2, 89, 58, music);
            break;
        case 1:
            u8g2.drawXBMP(0, 3, 89, 57, settings);
            break;
        case 2:
            u8g2.drawXBMP(0, 3, 89, 57, now_playing);
            break;
        case 3:
            u8g2.drawXBMP(0, 3, 89, 58, shuffle);
            break;
    }

    u8g2.sendBuffer();
}

void MainMenu(char cmd) {
    switch(cmd) {
        case 'z':
            MainMenuDraw();
            break;
        case 'd':
            if(main_menu_pos < 3) {
                main_menu_pos++;
                MainMenuDraw();
            }
            break;
        case 'u':
            if(main_menu_pos > 0) {
                main_menu_pos--;
                MainMenuDraw();
            }
            break;
        case 'e':
            ChangeMainMenu();
            break;
    } 
}

void SettingsMenuDraw() {
    u8g2.clearBuffer();
    SidebarDraw();

    switch(settings_menu_pos) {
        case 0:
        u8g2.drawXBMP(0, 0, 91, 64, about);
        break;
        case 1:
        u8g2.drawXBMP(0, 0, 91, 64, BT);
        break;
        case 2:
        u8g2.drawXBMP(0, 0, 91, 64, EQ_scr);
        break;
        case 3:
        u8g2.drawXBMP(0, 0, 91, 64, database);
        break;
    }

    u8g2.sendBuffer();
}

void SettingsMenu(char cmd) {
    switch(cmd) {
        case 'b':
            selected_menu = 0;
            MenuInput('z');
            break;
        case 'd':
            if(settings_menu_pos < 3) {
                settings_menu_pos++;
            }
            break;
        case 'u':
            if(settings_menu_pos > 0) {
                settings_menu_pos--;
            }
            break;
        case 'e':
            ChangeSettingsMenu();
            break;
        default:
            break;
    }
    if(cmd != 'b') {
        SettingsMenuDraw();
    }
}


void MenuInput(char cmd) {
    switch(selected_menu) {
        case 0:
            MainMenu(cmd);
            break;
        case 1:
            break;
        case 2:
            SettingsMenu(cmd);
            break;
        case 3:
            break;
        case 4:
            break;
    }
}

void ChangeMainMenu() {
    switch(main_menu_pos) {
        case 0:
            selected_menu = 1;
            break;
        case 1:
            selected_menu = 2;
            break;
        case 2:
            selected_menu = 3;
            break;
        case 3:
            selected_menu = 4;
            break;
    }
    MenuInput('z');
}

void ChangeSettingsMenu() {
    switch(settings_menu_pos) {
        case 0:
            selected_menu = 5;
            break;
        case 1:
            selected_menu = 6;
            break;
        case 2:
            selected_menu = 7;
            break;
        case 3:
            selected_menu = 8;
            break;
    }
    MenuInput('z');
}

void setup() {
  SetupDisplay();
}

void loop() {
    delay(1000);
    MenuInput('d');
    delay(1000);
    MenuInput('d');
    delay(1000);
    MenuInput('d');
    delay(1000);
    MenuInput('u');
    delay(1000);
    MenuInput('u');
    delay(1000);
    MenuInput('u');
    delay(1000);
    MenuInput('d');
    delay(1000);
    MenuInput('e');
    delay(1000);
    MenuInput('d');
    delay(1000);
    MenuInput('d');
    delay(1000);
    MenuInput('d');
    delay(1000);
    MenuInput('u');
    delay(1000);
    MenuInput('u');
    delay(1000);
    MenuInput('u');
    delay(1000);
    MenuInput('b');
    delay(1000);
    MenuInput('u');
}