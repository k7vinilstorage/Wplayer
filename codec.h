#ifndef Codec_H
#define Codec_H

#include <SparkFun_WM8960_Arduino_Library.h> 

WM8960 code;

class IpodCodec {
    public:
        IpodCodec();
        void StartCodec();
        void ChangeVol();
    private:
        float vol = -25.00;        
};


#endif