#ifndef Data_H
#define Data_H

#include <sqlite3.h>
#include <SD.h>
#include <SPI.h>
#include <string.h>

class IpodData {
    public:
        IpodData();

        bool sd_state = false;

    private:
    
};


#endif