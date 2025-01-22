#ifndef Inputs_H
#define Inputs_H

#include "Arduino.h"


class IpodInputs {
    public:
        IpodInputs();
        void InitInputs();
        char DetecctInput();
        
    private:
        #define down_btn 26
        #define up_btn 4
        #define prev_btn 27
        #define next_btn 21
        #define enter_btn 13
        #define back_btn  12
        #define play_pause_btn 17
        
        unsigned long timing;
        unsigned long btn_time;

        int down_btn_state = 1;
        int down_btn_state_ = 1;

        int up_btn_state = 1;
        int up_btn_state_ = 1;

        int enter_btn_btn_state = 1;
        int enter_btn_btn_state_ = 1;

        int back_btn_btn_state = 1;
        int back_btn_btn_state_ = 1;

        int play_pause_btn_state = 1;
        int play_pause_btn_state_ = 1;

        int prev_btn_state = 1;
        int prev_btn_state_ = 1;

        int next_btn_state = 1;
        int next_btn_state_ = 1;

};

#endif