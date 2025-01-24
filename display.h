#ifndef Display_H
#define Display_H

#include <U8g2lib.h>
#include <Wire.h>
#include "images.h"
#include "player.h"

class IpodDisplay {
    public:
        IpodDisplay(U8G2_SSD1306_128X64_NONAME_F_HW_I2C &display, IpodPlayer &p_class, IpodData & d_class);

        void SetupDisplay();
        void MenuInput(char cmd);

        int music_select = 0;

    private:
        U8G2_SSD1306_128X64_NONAME_F_HW_I2C *u8g2;
        IpodData *data;
        IpodPlayer *player;

        int main_menu_pos = 0;
        int settings_menu_pos = 0;
        int eq_menu_pos = 0;
        int music_menu_pos = 0;
        int selected_menu = 0;
        int sel_pos = 6;

        char *song_path;
        char *song_name;
        char *song_artist;

        void SdStateDraw();
        void PlayingIndicator();
        void SidebarDraw();
        void MainMenuDraw();
        void MainMenu(char cmd);
        void SettingsMenuDraw();
        void SettingsMenu(char cmd);
        void MusicMenuDraw();
        void MusicMenu(char cmd);
        void PlayMenuDraw();
        void PlayMenu(char cmd);
        void AboutMenuDraw();
        void AboutMenu(char cmd);
        void EQMenuDraw();
        void EQMenu(char cmd);
        void ChangeMainMenu();
        void ChangeSettingsMenu();
};


#endif