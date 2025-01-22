#ifndef Player_H
#define Player_H

#include "data.h"
#include <SparkFun_WM8960_Arduino_Library.h>
#include "AudioTools.h"
#include "AudioCodecs/CodecMP3Helix.h"
#include <Arduino.h>

class IpodPlayer {
    public:
        IpodPlayer();
        void ChangeVol(float vol);
        bool StreamAudio();
        void PlayPause();
        void Stop();
        void Play(char *file);

        bool is_playing = false;
        int playing_song;

        void SetupDac();
        
        
    private:
        void SetupPlayer();

        WM8960 dac;
        I2SStream i2s; 
        Equilizer3Bands eq;
        ConfigEquilizer3Bands cfg_eq;
        EncodedAudioStream decoder;
        StreamCopy copier;
        File audioFile;
};


#endif