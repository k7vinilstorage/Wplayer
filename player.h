#ifndef Player_H
#define Player_H

#include "data.h"
#include <SparkFun_WM8960_Arduino_Library.h>
#include "AudioTools.h"
#include "AudioCodecs/CodecMP3Helix.h"
#include <Arduino.h>

class IpodPlayer {
    public:
        IpodPlayer(IpodData & d_class);
        void ChangeVol();
        bool StreamAudio();
        void PlayPause();
        void Stop();
        void Play(int id);
        void Shuflle(int song_count);

        bool is_playing = false;
        char *song_path;
        int playing_song;

        std::vector<int> random_song_ids;
        char player_mode = 'n';
        int suffle_id = 0;

        void SetupDac();
        void EQUpdate();
        float vol;
        float eq_settings[3]  = {1, 1, 1};
        int eq_select = 0; 
        
        
    private:
        IpodData *data;

        void SetupPlayer();

        WM8960 dac;
        I2SStream i2s; 
        EncodedAudioStream decoder;
        StreamCopy copier;
        File audioFile;
        Equilizer3Bands eq;
        ConfigEquilizer3Bands cfg_eq;
};


#endif