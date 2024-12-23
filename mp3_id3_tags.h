#ifndef MP3_ID3_TAGS_H
#define MP3_ID3_TAGS_H

#include <Arduino.h>
#include <SD.h>

// Structure to hold the title, artist, and album
typedef struct {
    char title[31];
    char artist[31];
    char album[31];
} mp3_id3_tags;

// Declaration of the function to read the ID3v1 tags from an MP3 file
bool mp3_id3_file_read_tags(File *mp3File, mp3_id3_tags *tags);

#endif // MP3_ID3_TAGS_H
