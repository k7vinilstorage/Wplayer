#include "player.h"

IpodPlayer::IpodPlayer() : eq(i2s), decoder(&eq, new MP3DecoderHelix()), copier(i2s, eq) {
    SetupPlayer();
}

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

void IpodPlayer::SetupPlayer() {
    auto cfg = i2s.defaultConfig(TX_MODE);
    cfg.buffer_size = 4096;
    cfg.bits_per_sample = 16;
    cfg.channels = 2;
    cfg.sample_rate = 44100;
    decoder.begin();
    i2s.begin(cfg);
    i2s.end();
    copier.begin(decoder, audioFile);

    cfg_eq = eq.defaultConfig();
    cfg_eq.setAudioInfo(cfg); // use channels, bits_per_sample and sample_rate from kit
    cfg_eq.gain_low = 1; 
    cfg_eq.gain_medium = 1;
    cfg_eq.gain_high = 1;
    eq.begin(cfg_eq);
}   

void IpodPlayer::ChangeVol(float vol) {
    dac.setHeadphoneVolumeDB(vol);
}

void IpodPlayer::PlayPause() {
    if(is_playing) {
        is_playing = false;
        i2s.end();
    }
    else {
        is_playing = true;
        i2s.begin();
    }
}

void IpodPlayer::Stop() {
    if(is_playing) {
        dac.enableDacMute();
        is_playing = false;
        decoder.flush();
        decoder.end();
        audioFile.flush();
        audioFile.close();
        i2s.end();
    }
}

void IpodPlayer::Play(char *file) {
    Stop();
    audioFile = SD.open(file);
    if(audioFile) {
        is_playing = true;
    }
    else {
        Serial.println("Error Opening audio file");
    }
    decoder.begin();
    i2s.begin();
    dac.disableDacMute();
}

bool IpodPlayer::StreamAudio() {
    if(is_playing) {
        if(!copier.copy()) {
            return false;
            //add player modes;
        }
        else{
            return true;
        }
    }
    else {
      return false;
    }
}


