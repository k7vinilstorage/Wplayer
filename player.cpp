#include "player.h"

IpodPlayer::IpodPlayer() {}

void IpodPlayer::SetupDac() {
    if (dac.begin() == false) {
        while(1) {
            Serial.println("Error starting up dac");
            delay(1000);
        }
    }

    dac.enableVREF();
    dac.enableVMID();
    dac.enableLD2LO();
    dac.enableRD2RO();
    dac.setLB2LOVOL(WM8960_OUTPUT_MIXER_GAIN_NEG_3DB); 
    dac.setRB2ROVOL(WM8960_OUTPUT_MIXER_GAIN_NEG_3DB);
    dac.enableLOMIX(); 
    dac.enableROMIX();
    dac.enablePLL(); 
    dac.setPLLPRESCALE(WM8960_PLLPRESCALE_DIV_2);
    dac.setSMD(WM8960_PLL_MODE_FRACTIONAL);
    dac.setCLKSEL(WM8960_CLKSEL_PLL);
    dac.setSYSCLKDIV(WM8960_SYSCLK_DIV_BY_2);
    dac.setBCLKDIV(4);
    dac.setDCLKDIV(WM8960_DCLKDIV_16);
    dac.setPLLN(7);
    dac.setPLLK(0x86, 0xC2, 0x26);
    dac.setWL(WM8960_WL_16BIT);
    dac.enablePeripheralMode();
    dac.enableDacRight();
    dac.enableDacLeft();
    dac.disableLoopBack();
    dac.enableDacMute();
    dac.enableHeadphones();
    dac.enableHeadphoneStandby();
    dac.enableOUT3MIX(); 
    dac._writeRegisterBit(0x06, 1, 1);
    dac._writeRegisterBit(0x06, 3, 1);
    dac.enable3d();
    dac.set3dDepth(7);
    dac.setHeadphoneVolumeDB(-25.00);
}

void IpodPlayer::change_vol(float vol) {
  dac.setHeadphoneVolumeDB(vol);
}