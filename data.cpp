#include "data.h"

IpodData::IpodData() {
    if(!SD.begin()) {
        Serial.println("Error Starting SD");
    }
}