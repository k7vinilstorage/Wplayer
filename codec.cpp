#include "codec.h"

IpodCodec::IpodCodec() {}


void StartCodec() {

  if (code.begin() == false) {
    while (1) {
      Serial.println("Error starting up code");
      delay(1000);
    }
  }

  code.enableVREF();
  code.enableVMID();
  code.enableLD2LO();
  code.enableRD2RO();
  code.setLB2LOVOL(WM8960_OUTPUT_MIXER_GAIN_NEG_3DB); 
  code.setRB2ROVOL(WM8960_OUTPUT_MIXER_GAIN_NEG_3DB);
  code.enableLOMIX(); 
  code.enableROMIX();
  code.enablePLL(); 
  code.setPLLPRESCALE(WM8960_PLLPRESCALE_DIV_2);
  code.setSMD(WM8960_PLL_MODE_FRACTIONAL);
  code.setCLKSEL(WM8960_CLKSEL_PLL);
  code.setSYSCLKDIV(WM8960_SYSCLK_DIV_BY_2);
  code.setBCLKDIV(4);
  code.setDCLKDIV(WM8960_DCLKDIV_16);
  code.setPLLN(7);
  code.setPLLK(0x86, 0xC2, 0x26);
  code.setWL(WM8960_WL_16BIT);
  code.enablePeripheralMode();
  code.enableDacRight();
  code.enableDacLeft();
  code.disableLoopBack();
  code.enableDacMute();
  code.enableHeadphones();
  code.enableHeadphoneStandby();
  code.enableOUT3MIX(); 
  code._writeRegisterBit(0x06, 1, 1);
  code._writeRegisterBit(0x06, 3, 1);
  code.enable3d();
  code.set3dDepth(7);
  code.setHeadphoneVolumeDB(-25.00);
}

void ChangeVol(float vol) {
  code.setHeadphoneVolumeDB(vol);
}