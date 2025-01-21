#include "display.h"

IpodDisplay::IpodDisplay(U8G2_SSD1306_128X64_NONAME_F_HW_I2C &display, IpodPlayer &p_class, IpodData & d_class) {
    Wire.begin(33,32);
    u8g2 = &display;
    data = &d_class;
    player = &p_class;
}


void IpodDisplay::SetupDisplay() {
    u8g2->begin();
    u8g2->clearBuffer();
    u8g2->setFontMode(1);
    u8g2->setBitmapMode(1);
    u8g2->drawXBMP(1, 0, 127, 64, ipod_w);
    u8g2->sendBuffer();
    delay(2000);
    MainMenu('z');
}

void IpodDisplay::SdStateDraw() {
    if(data->sd_state) {
        u8g2->drawXBMP(112, 46, 12, 15, image_micro_sd_bits);
    }
    else {
        u8g2->drawXBMP(111, 46, 14, 15, image_micro_sd_no_card_bits);
    }
}

void IpodDisplay::PlayingIndicator() {
    if(player->is_playing) {
        u8g2->drawXBMP(103, 23, 15, 15, image_music_play_bits);
    }
    else {
        u8g2->drawXBMP(103, 23, 15, 15, image_music_pause_bits);
    }
}

void IpodDisplay::SidebarDraw() {
    SdStateDraw();
    PlayingIndicator();
    u8g2->drawXBMP(94, 46, 14, 15, image_bluetooth_not_connected_bits);
    u8g2->drawXBMP(97, 3, 24, 13, image_battery_empty_bits);
    u8g2->drawLine(89, 0, 89, 63);
    u8g2->drawLine(90, 64, 90, 0);
}

void IpodDisplay::MainMenuDraw() {
    u8g2->clearBuffer();
    u8g2->setFont(u8g2_font_6x10_tr);
    SidebarDraw();

    switch(main_menu_pos) {
        case 0:
            u8g2->drawXBMP(0, 2, 89, 58, music);
            break;
        case 1:
            u8g2->drawXBMP(0, 3, 89, 57, settings);
            break;
        case 2:
            u8g2->drawXBMP(0, 3, 89, 57, now_playing);
            break;
        case 3:
            u8g2->drawXBMP(0, 3, 89, 58, shuffle);
            break;
    }

    u8g2->sendBuffer();
}

void IpodDisplay::MainMenu(char cmd) {
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

void IpodDisplay::SettingsMenuDraw() {
    u8g2->clearBuffer();
    SidebarDraw();

    switch(settings_menu_pos) {
        case 0:
        u8g2->drawXBMP(0, 0, 91, 64, about);
        break;
        case 1:
        u8g2->drawXBMP(0, 0, 91, 64, BT);
        break;
        case 2:
        u8g2->drawXBMP(0, 0, 91, 64, EQ_scr);
        break;
        case 3:
        u8g2->drawXBMP(0, 0, 91, 64, database);
        break;
    }

    u8g2->sendBuffer();
}

void IpodDisplay::SettingsMenu(char cmd) {
    switch(cmd) {
        case 'b':
            selected_menu = 0;
            MenuInput('z');
            break;
        case 'd':
            if(settings_menu_pos < 3) {
                settings_menu_pos++;
            }
            SettingsMenuDraw();
            break;
        case 'u':
            if(settings_menu_pos > 0) {
                settings_menu_pos--;
            }
            SettingsMenuDraw();
            break;
        case 'e':
            ChangeSettingsMenu();
            SettingsMenuDraw();
            break;
        default:
            break;
    }
}

void IpodDisplay::MenuInput(char cmd) {
    switch(selected_menu) {
        case 0:
            MainMenu(cmd);
            break;
        case 1:
            MusicMenu(cmd);
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

void IpodDisplay::MusicMenuDraw() {
    u8g2->clearBuffer();
    u8g2->drawXBMP(121, sel_pos, 4, 7, image_ButtonLeft__copy__bits);
    u8g2->drawStr(3, 13, data->songList[music_menu_pos]);
    u8g2->drawStr(3, 25, data->songList[music_menu_pos + 1]);
    u8g2->drawStr(3, 37, data->songList[music_menu_pos + 2]);
    u8g2->drawStr(3, 49, data->songList[music_menu_pos + 3]);
    u8g2->drawStr(3, 61, data->songList[music_menu_pos + 4]);
    u8g2->sendBuffer();
}

void IpodDisplay::MusicMenu(char cmd) {
    switch(cmd) {
        case 'z':
            MusicMenuDraw();
            break;
        case 'd':
            if(music_select < (data->song_count - 1)) {
                music_select++;
                if(sel_pos < 54) {
                sel_pos = sel_pos + 12;
                }
                else {
                music_menu_pos++;
                }
            }
            Serial.print("sel: ");
            Serial.println(music_select);
            Serial.print("pos: ");
            Serial.println(music_menu_pos);
            MusicMenuDraw();
            break;
        case 'u':
            if(music_select > 0) {
                music_select--;
                if(sel_pos > 6) {
                sel_pos = sel_pos - 12;
                }
                else {
                music_menu_pos--;
                }
            }
            Serial.print("sel: ");
            Serial.println(music_select);
            Serial.print("pos: ");
            Serial.println(music_menu_pos);
            MusicMenuDraw();
            break;
        case 'e':
            break;
        case 'b':
            selected_menu = 0;
            MenuInput('z');
            break;
    }
  
    MusicMenuDraw();
}

void IpodDisplay::ChangeMainMenu() {
    selected_menu = main_menu_pos + 1;
    MenuInput('z');
}

void IpodDisplay::ChangeSettingsMenu() {
    selected_menu = settings_menu_pos + 5;
    MenuInput('z');
}