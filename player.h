#ifndef Player_H
#define Player_H

#include "data.h"
#include <SparkFun_WM8960_Arduino_Library.h>
#include "AudioTools.h"
#include "AudioCodecs/CodecMP3Helix.h"

class IpodPlayer {
    public:
        IpodPlayer();
        void SetupDac();
        void change_vol(float vol);
        

        bool is_playing = false;
    private:
        WM8960 dac;
        
        I
};


#endif