#include "player.h"

IpodPlayer::IpodPlayer(IpodData & d_class) : eq(i2s), decoder(&eq, new MP3DecoderHelix()), copier(i2s, eq) {
    data = &d_class;
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

void IpodPlayer::ChangeVol() {
    if(vol < -74.00) {
        vol = -74.00;
    }
    if(vol > 0.00) {
        vol = 0.00;
    }
    dac.setHeadphoneVolumeDB(vol);
    delay(10);
    data->VolSave(vol);
}

void IpodPlayer::PlayPause() {
    if(is_playing) {
        is_playing = false;
        dac.enableDacMute();
        i2s.end();
    }
    else if(never_played) {
        Play(playing_song);
    }
    else {
        is_playing = true;
        i2s.begin();
        dac.disableDacMute();
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

void IpodPlayer::Play(int id) {
    if(never_played) {
        never_played = false;
    }
    playing_song = id;
    song_path = data->RequestItem(id, 'P');
    Stop();
    audioFile = SD.open(song_path);
    if(audioFile) {
      decoder.begin();
      i2s.begin();
      dac.disableDacMute();
      delay(100);
      is_playing = true;
    }
    else {
        Serial.println("Error Opening audio file");
    }
    free(song_path);
    data->MusicSave(playing_song);
}

void IpodPlayer::Shuflle(int song_count) {
    random_song_ids.clear();

    for(int i = 0; i < song_count; i++) {
        random_song_ids.push_back(i);
    }

    for(int i = song_count; i > 0; i--) {
        int j = random(i + 1);
        int temp = random_song_ids[i];
        random_song_ids[i] = random_song_ids[j];
        random_song_ids[j] = temp;
    }
    player_mode = 's';
}

void IpodPlayer::EQUpdate() {
    eq.end();
    cfg_eq.gain_low = eq_settings[0];
    cfg_eq.gain_medium = eq_settings[1];
    cfg_eq.gain_high = eq_settings[2];
    eq.begin(cfg_eq);
    data->EQSave(cfg_eq.gain_low, cfg_eq.gain_medium, cfg_eq.gain_high);
}

bool IpodPlayer::StreamAudio() {
    if(is_playing) {
        if(!copier.copy()) {
            return false;
        }
        else{
            return true;
        }
    }
    else {
      return false;
    }
}


