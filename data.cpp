#include "data.h"

IpodData::IpodData() {}

void IpodData::SdInit() {
  if(!SD.begin()) {
        Serial.println("Error Starting SD");
    }
    else {
        cardSize = SD.cardSize();
        cardSize = cardSize / (1024 * 1024 * 1024);
        float cardUsed = SD.usedBytes();
        cardUsed = cardUsed / (1024 * 1024 * 1024);
        cardFree = cardSize - cardUsed;
        Serial.println(cardSize);
        Serial.println(cardFree);
        sd_state = true;
  }
}

void IpodData::UpDatabase(const char *dirname, uint8_t levels) {
    File root;

    root = SD.open(dirname);
   
    if (!root) {
        Serial.println("No root");
        return;
    }

    if (!root.isDirectory()) {
        Serial.println("No root dir");
        return;
    }

    File file = root.openNextFile();

    while(file) {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        if(file.isDirectory()) {
            if(levels) {
                UpDatabase(file.path(), levels - 1);
            }
        } 
        else {
            if (mp3_id3_file_read_tags(&file, &tags)) {
                AddItem(song_count, tags.title, tags.artist, tags.album, file.path());
                song_count++;
            } 
            else {
                AddItem(song_count, file.name(), "Unknow", "Unknow", file.path());
                song_count++;
            }
            Serial.print(song_count);
        }
        file = root.openNextFile();
    }
}

void IpodData::DbInit(const char *path) {
    const char *sql = "CREATE TABLE IF NOT EXISTS MUSIC(Id INTEGER PRIMARY KEY, Name TEXT, Artist TEXT, Album TEXT, Path TEXT);";
    const char *sql2 = "CREATE TABLE IF NOT EXISTS CONFIG(Id INTEGER PRIMARY KEY, Vol FLOAT, Mus INT, Bass FLOAT, Middle FLOAT, Treble FLOAT);";
    const char *sql3 = "INSERT INTO CONFIG VALUES (1, -35.00, 1, 1, 1, 1);";

    rc = sqlite3_open(path, &db);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %    int count = 0;s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
    }

    rc = sqlite3_exec(db, sql2, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %    int count = 0;s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
    }

    rc = sqlite3_exec(db, sql3, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %    int count = 0;s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
    }

    CountSongs();
    for(int i = 0; i < song_count; i++) {
        songList.push_back(RequestItem(i, 'N'));
        Serial.println(songList[i]);
    }
}

void IpodData::AddItem(int id, const char *name, const char *artist, const char *album, const char *path) {
    const char *sql = "INSERT INTO MUSIC VALUES (?, ?, ?, ?, ?)";
    sqlite3_stmt *stmt;

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, id);
    sqlite3_bind_text(stmt, 2, name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, artist, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, album, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, path, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Execution failed: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Record inserted successfully.\n");
    }

    sqlite3_finalize(stmt);
}

char *IpodData::RequestItem(int id, char request) {
    const char *sql;
    char *result = NULL;
    sqlite3_stmt *stmt;

    switch(request) {
        case 'N':
            sql = "SELECT Name FROM MUSIC WHERE Id = ?";
            break;
        case 'A':
            sql = "SELECT Artist FROM MUSIC WHERE Id = ?";
            break;
        case 'B':
            sql = "SELECT Album FROM MUSIC WHERE Id = ?";
            break;
        case 'P':
            sql = "SELECT Path FROM MUSIC WHERE Id = ?";
            break;
        default:
            return NULL; // Invalid request type
    }

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *value = (const char *)sqlite3_column_text(stmt, 0);
        result = strdup(value); // Duplicate the string to return it
    } else {
        fprintf(stderr, "No data found for Id: %d\n", id);
    }

    sqlite3_finalize(stmt);
    return result;
}

void IpodData::CountSongs() {
    const char *sql = "SELECT COUNT(*) FROM MUSIC;";
    sqlite3_stmt *stmt;

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        song_count = sqlite3_column_int(stmt, 0); // Get the count value
    } else {
        fprintf(stderr, "Failed to get order count.\n");
    }

    sqlite3_finalize(stmt);
}

void IpodData::VolSave() {
  
}

void IpodData::EQSave() {
  
}

void IpodData::MusicSave() {
  
}

void IpodData::DeleteDatabase(){
    rc = sqlite3_exec(db, "DROP TABLE IF EXISTS MUSIC", 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
    }

    rc = sqlite3_exec(db, "CREATE TABLE MUSIC(Id INTEGER PRIMARY KEY, Name TEXT, Artist TEXT, Album TEXT, Path TEXT);", 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
    }

    song_count = 0;
}