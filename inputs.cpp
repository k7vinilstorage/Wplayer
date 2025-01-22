#include "inputs.h"

IpodInputs::IpodInputs() {}

void IpodInputs::InitInputs() {
    pinMode(play_pause_btn, INPUT_PULLUP);  
    pinMode(down_btn, INPUT_PULLUP);
    pinMode(up_btn, INPUT_PULLUP);
    pinMode(enter_btn, INPUT_PULLUP);
    pinMode(back_btn, INPUT_PULLUP);
    pinMode(prev_btn, INPUT_PULLUP);
    pinMode(next_btn, INPUT_PULLUP);
}

char IpodInputs::DetecctInput() {
    timing = millis();
    char btn = '0';

    down_btn_state = digitalRead(down_btn);
    if(down_btn_state == 0 && down_btn_state_ == 1) {
        btn = 'd';
        btn_time = millis();
    }
    if(down_btn_state_ != down_btn_state) {
        down_btn_state_ = down_btn_state;
    }

    if(down_btn_state == 0 && down_btn_state == down_btn_state_) {
        if(timing > btn_time + 1000) {
        btn = 'd';
        }
    }

    //up
    up_btn_state = digitalRead(up_btn);
    if(up_btn_state == 0 && up_btn_state_ == 1) {
        btn = 'u';
        btn_time = millis();
    }
    if(up_btn_state_ != up_btn_state) {
        up_btn_state_ = up_btn_state;
    }

    if(up_btn_state == 0 && up_btn_state == up_btn_state_) {
        if(timing > btn_time + 1000) {
        btn = 'u';
        }
    }

    //prev
    prev_btn_state = digitalRead(prev_btn);
    if(prev_btn_state == 0 && prev_btn_state_ == 1) {
        btn = 'a';
    }
    if(prev_btn_state_ != prev_btn_state) {
        prev_btn_state_ = prev_btn_state;
    }

    //next
    next_btn_state = digitalRead(next_btn);
    if(next_btn_state == 0 && next_btn_state_ == 1) {
        btn = 'n';
    }
    if(next_btn_state_ != next_btn_state) {
        next_btn_state_ = next_btn_state;
    }

    //enter_btn
    enter_btn_btn_state = digitalRead(enter_btn);
    if(enter_btn_btn_state == 0 && enter_btn_btn_state_ == 1) {
        btn = 'e';
    }
    if(enter_btn_btn_state_ != enter_btn_btn_state) {
        enter_btn_btn_state_ = enter_btn_btn_state;
    }

    //back_btn
    back_btn_btn_state = digitalRead(back_btn);
    if(back_btn_btn_state == 0 && back_btn_btn_state_ == 1) {
        btn = 'b';
    }
    if(back_btn_btn_state_ != back_btn_btn_state) {
        back_btn_btn_state_ = back_btn_btn_state;
    }

    //play_pause
    play_pause_btn_state = digitalRead(play_pause_btn);
    if(play_pause_btn_state == 0 && play_pause_btn_state_ == 1) {
        btn = 'p';
    }
    if(play_pause_btn_state_ != play_pause_btn_state) {
        play_pause_btn_state_ = play_pause_btn_state;
    }

    return btn;
}