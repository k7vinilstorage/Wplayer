#ifndef Data_H
#define Data_H

#include <sqlite3.h>
#include <SD.h>
#include <SPI.h>
#include <string.h>
#include <vector>
#include "mp3_id3_tags.h"

class IpodData {
    public:
        IpodData();
        void DataInit();
        void UpDatabase(const char *dirname, uint8_t levels);
        void DeleteDatabase();
        char *RequestItem(int id, char request);
        void VolSave(float vol);
        void EQSave(float bass, float middle, float treble);
        float GetVolume();
        float GetTreble();
        float GetBass();
        float GetMiddle();
        
        std::vector<const char*> songList;
        
        bool sd_state = false;
        float cardSize;
        float cardFree;
        int song_count = 0;
        
        
        private:
        
        void AddItem(int id, const char *name, const char *artist, const char *album, const char *path);
        void CountSongs();
        
        void SdInit();
        void DbInit(const char *path);
        
        
        sqlite3 *db;
        mp3_id3_tags tags;
        char *err_msg = 0;
        int rc;
        char *song_name[15];
        char *song_artist[15];
        char *song_path[15];
    };


#endif