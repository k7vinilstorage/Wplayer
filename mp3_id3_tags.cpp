#include "mp3_id3_tags.h"

// Function to read the ID3v1 tags from an MP3 file
bool mp3_id3_file_read_tags(File *mp3File, mp3_id3_tags *tags) {
    if (!mp3File->seek(mp3File->size() - 128)) {
        return false; // Failed to seek to the last 128 bytes where ID3v1 tags reside
    }

    char buffer[128];
    if (mp3File->read((uint8_t *)buffer, 128) != 128) {
        return false; // Failed to read the last 128 bytes
    }

    if (strncmp(buffer, "TAG", 3) != 0) {
        return false; // No ID3v1 tag found
    }

    // Copy title (30 characters)
    strncpy(tags->title, buffer + 3, 30);
    tags->title[30] = '\0'; // Null-terminate the string

    // Copy artist (30 characters)
    strncpy(tags->artist, buffer + 33, 30);
    tags->artist[30] = '\0'; // Null-terminate the string

    // Copy album (30 characters)
    strncpy(tags->album, buffer + 63, 30);
    tags->album[30] = '\0'; // Null-terminate the string

    return true; // Successfully read the ID3v1 tags
}
