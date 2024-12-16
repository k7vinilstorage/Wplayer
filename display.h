#ifndef Display_H
#define Display_H

#include <U8g2lib.h>
#include <Wire.h>
#include "images.h"
#include "player.h"

class IpodDisplay {
    public:
        IpodDisplay(U8G2_SSD1306_128X64_NONAME_F_HW_I2C &display, IpodData &SD, IpodPlayer &PLAY);

        void SetupDisplay();
        void MenuInput(char cmd);

    private:
        U8G2_SSD1306_128X64_NONAME_F_HW_I2C *u8g2;
        IpodData *data;
        IpodPlayer *player;

        int main_menu_pos = 0;
        int settings_menu_pos = 0;
        int selected_menu = 0;


        void SdDraw();
        void PlayingIndicator();
        void SidebarDraw();
        void MainMenuDraw();
        void MainMenu(char cmd);
        void SettingsMenuDraw();
        void SettingsMenu(char cmd);
        void ChangeMainMenu();
        void ChangeSettingsMenu();



};


#endif