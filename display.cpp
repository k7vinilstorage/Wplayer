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
}

void IpodDisplay::SdStateDraw() {
    if(data->sd_state) {
        u8g2->drawXBMP(112, 46, 12, 15, image_micro_sd_bits);
    }
    else {
        u8g2->drawXBMP(111, 46, 14, 15, image_micro_sd_no_card_bits);
    }
}

void IpodDisplay::VolStateDraw() {
    if(player->vol < -58) {
        u8g2->drawXBM(110, 3, 15, 16, image_network_bits);
    }
    else if(player->vol < -42) {
        u8g2->drawXBM(110, 3, 15, 16, image_network_1_bar_bits);
    }
    else if(player->vol < -26) {
        u8g2->drawXBM(110, 3, 15, 16, image_network_2_bars_bits);
    }
    else if(player->vol < -10) {
        u8g2->drawXBM(110, 3, 15, 16, image_network_3_bars_bits);
    }
    else {
        u8g2->drawXBM(110, 3, 15, 16, image_network_4_bars_bits);
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
        case 'z':
            SettingsMenuDraw();
            break;
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
            break;
        default:
            break;
    }
}

void IpodDisplay::MenuInput(char cmd) {
    if(cmd == 'p') {
        player->PlayPause();
        MenuInput('z');
    }
    else {
        if(selected_menu != 7 && (cmd == 'n' || cmd == 'a') && player->is_playing) {
            if(cmd == 'n') {
                if(player->player_mode == 's') {
                    if(player->suffle_id < data->song_count) {
                        player->suffle_id++;
                        player->Play(player->random_song_ids[player->suffle_id]);
                        MenuInput('z');
                    }
                }
                else {
                    if(player->playing_song < data->song_count) {
                        player->playing_song++;
                        player->Play(player->playing_song);
                        MenuInput('z');
                    }
                }
            }
            else {
                if(player->player_mode == 's') {
                    if(player->suffle_id > 0) {
                        player->suffle_id--;
                        player->Play(player->random_song_ids[player->suffle_id]);
                        MenuInput('z');
                    }
                }
                else {
                    if(player->playing_song > 0) {
                        player->playing_song--;
                        player->Play(player->playing_song);
                        MenuInput('z');
                    }

                }
            }
        }
        else {
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
                    PlayMenu(cmd);
                    break;
                case 4:
                    player->Shuflle(data->song_count);
                    player->Play(player->random_song_ids[0]);
                    selected_menu = 3;
                    prev_selected_menu = 0;
                    MenuInput('z');
                    break;
                case 5:
                    AboutMenu(cmd);
                    break;
                case 6:
                    selected_menu = 2;
                    break;
                case 7:
                    EQMenu(cmd);
                    break;
                case 8:
                    u8g2->clearBuffer();
                    u8g2->setFontMode(1);
                    u8g2->setBitmapMode(1);
                    u8g2->drawXBMP(46, 8, 36, 29, image_Restoring_bits);
                    u8g2->drawXBMP(16, 45, 95, 11, update);
                    u8g2->sendBuffer();
                    data->DeleteDatabase();
                    data->UpDatabase("/Music", 2);
                    ESP.restart();
                    break;
            }
        }
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
            player->player_mode = 'n';
            player->Play(music_select);
            selected_menu = 3;
            prev_selected_menu = 1;
            MenuInput('z');
            break;
        case 'b':
            selected_menu = 0;
            MenuInput('z');
            break;
    }
}

void IpodDisplay::PlayMenuDraw() {
  u8g2->clearBuffer();
  u8g2->setFontMode(1);
  u8g2->drawLine(127, 19, 0, 19);
  u8g2->setFont(u8g2_font_6x13_tr);
  free(song_name);
  free(song_artist);
  song_name = data->RequestItem(player->playing_song, 'N');
  song_artist = data->RequestItem(player->playing_song, 'A');
  u8g2->drawStr(1, 37, song_name);
  u8g2->drawStr(1, 51, song_artist);
  u8g2->setFont(u8g2_font_6x10_tr);
  u8g2->drawStr(31, 13, "Now Playing");
  if(player->is_playing) {
    u8g2->drawXBMP(2, 2, 15, 15, image_music_play_bits);
  }
  else {
    u8g2->drawXBMP(2, 2, 12, 15, image_music_pause_bits);
  }
  VolStateDraw();
  u8g2->sendBuffer();
}

void IpodDisplay::PlayMenu(char cmd) {
    switch(cmd) {
        case 'z':
            PlayMenuDraw();
            break;
        case 'b':
            selected_menu = prev_selected_menu;
            MenuInput('z');
            break;
        case 'u':
            player->vol++;
            player->ChangeVol();
            PlayMenuDraw();
            break;
        case 'd':
            player->vol--;
            player->ChangeVol();
            PlayMenuDraw();
            break;
    }
}

void IpodDisplay::AboutMenu(char cmd) {
    if(cmd == 'b') {
        selected_menu = 2;
        MenuInput('z');
    }
    else {
        u8g2->clearBuffer();
        u8g2->setFont(u8g2_font_6x13_tr);
        u8g2->drawStr(35, 11, "About Wpod");
        u8g2->drawLine(0, 14, 127, 14);
        u8g2->drawStr(3, 27, "Songs");
        u8g2->drawStr(3, 59, "Version");
        u8g2->drawStr(3, 43, "Capacity");
        u8g2->setCursor(58, 45);
        u8g2->print(data->cardFree);
        u8g2->setCursor(96, 45);
        u8g2->print(data->cardSize);
        u8g2->setCursor(107, 29);
        u8g2->print(data->song_count);
        u8g2->drawStr(105, 60, "1.0");
        u8g2->drawLine(89, 44, 93, 36);
        u8g2->sendBuffer();
    }
}

void IpodDisplay::EQMenuDraw() {
    u8g2->clearBuffer();
    u8g2->setFont(u8g2_font_6x13_tr);
    u8g2->drawStr(57, 11, "EQ");
    u8g2->drawLine(0, 14, 127, 14);
    u8g2->drawStr(1, 47, "Middle");
    u8g2->drawStr(1, 33, "Bass");
    u8g2->drawStr(1, 61, "Treble");
    u8g2->setFont(u8g2_font_4x6_tr);
    u8g2->drawStr(45, 24, "-1.5     0     1.5");
    u8g2->drawXBMP(52, 42, 61, 3, eq_set);
    u8g2->drawLine((72 + (player->eq_settings[0] * 10)), 26, (72 + (player->eq_settings[0] * 10)), 32);
    u8g2->drawXBMP(52, 28, 61, 3, eq_set);
    u8g2->drawLine((72 + (player->eq_settings[1] * 10)), 40, (72 + (player->eq_settings[1] * 10)), 46);
    u8g2->drawLine((72 + (player->eq_settings[2] * 10)), 53, (72 + (player->eq_settings[2] * 10)), 59);
    u8g2->drawXBMP(52, 55, 61, 3, eq_set);
    switch(player->eq_select) {
        case 0:
            u8g2->drawXBMP(121, 26, 4, 7, image_ButtonLeft__copy__bits);
            break;
        case 1:
            u8g2->drawXBMP(121, 40, 4, 7, image_ButtonLeft__copy__bits);
            break;
        case 2:
            u8g2->drawXBMP(121, 53, 4, 7, image_ButtonLeft__copy__bits);
            break;
    }
    u8g2->sendBuffer();
    u8g2->setFont(u8g2_font_6x13_tr);
}

void IpodDisplay::EQMenu(char cmd) {
    switch(cmd) {
        case 'z':
            EQMenuDraw();
            break;
        case 'b':
            selected_menu = 2;
            MenuInput('z');
            break;
        case 'd':
            if(player->eq_select < 2) {
                player->eq_select++;
            }
            EQMenuDraw();
            break;    
        case 'u':
            if(player->eq_select > 0) {
                player->eq_select--;
            }
            EQMenuDraw();
            break;   
        case 'n':
            player->eq_settings[player->eq_select] = player->eq_settings[player->eq_select] + 0.1;
            player->EQUpdate();
            EQMenuDraw();
            break;
        case 'a':
            player->eq_settings[player->eq_select] = player->eq_settings[player->eq_select] - 0.1;
            player->EQUpdate();
            EQMenuDraw();
            break;
    }
}

void IpodDisplay::ChangeMainMenu() {
    selected_menu = main_menu_pos + 1;
    prev_selected_menu = 0;
    MenuInput('z');
}

void IpodDisplay::ChangeSettingsMenu() {
    selected_menu = settings_menu_pos + 5;
    MenuInput('z');
}