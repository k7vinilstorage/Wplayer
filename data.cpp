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
    
    const char* check_query = "SELECT 1 FROM CONFIG WHERE id = 1";
    sqlite3_stmt* stmt;
    int rc2 = sqlite3_prepare_v2(db, check_query, -1, &stmt, 0);

    if (rc2 != SQLITE_OK) {
        Serial.println("Failed to prepare SELECT statement");
        return;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {  // If no record is found, insert
        const char* insert_query = "INSERT INTO CONFIG VALUES (1, -35.00, 1, 1, 1, 1)";
        rc = sqlite3_exec(db, insert_query, 0, 0, 0);  // Execute the INSERT query
    
        if (rc != SQLITE_OK) {
            Serial.println("Error inserting data");
        } 
        else {
            Serial.println("Data inserted successfully");
        }
    } 
    else {
        Serial.println("Record with ID = 1 already exists");
    }

    // Finalize the statement to release resources
    sqlite3_finalize(stmt);


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

void IpodData::VolSave(float vol) {
    const char *sql = "UPDATE CONFIG SET Vol = ? WHERE Id = 1;";
    sqlite3_stmt *stmt;

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, vol);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Execution failed: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
}

void IpodData::EQSave(float bass, float middle, float treble) {
    const char *sql = "UPDATE CONFIG SET Bass = ?, Middle = ?, Treble = ? WHERE Id = 1;";
    sqlite3_stmt *stmt;

    // Prepare the SQL statement
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Bind the values for Bass, Middle, and Treble
    sqlite3_bind_double(stmt, 1, bass);
    sqlite3_bind_double(stmt, 2, middle);
    sqlite3_bind_double(stmt, 3, treble);

    // Execute the query
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Execution failed: %s\n", sqlite3_errmsg(db));
    }

    // Finalize the statement to release resources
    sqlite3_finalize(stmt);
}


void IpodData::MusicSave(int mus) {
    const char *sql = "UPDATE CONFIG SET Mus = ? WHERE Id = 1;";
    sqlite3_stmt *stmt;

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, mus);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Execution failed: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
}

float IpodData::GetVolume() {
    const char* query = "SELECT Vol FROM CONFIG WHERE id = 1";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);
    
    if (rc != SQLITE_OK) {
        // If preparing the SELECT statement fails, print the error
        Serial.print("Failed to prepare SELECT statement: ");
        Serial.println(sqlite3_errmsg(db));
        return -35.0f;  // Return -1 to indicate an error
    }

    // Execute the query
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        // If the record with id = 1 is found, return the value of Vol
        float vol = sqlite3_column_double(stmt, 0);  // Assuming Vol is in the first column
        sqlite3_finalize(stmt);  // Clean up the prepared statement
        return vol;
    } else {
        // If no record is found
        Serial.println("Record with ID = 1 not found");
        sqlite3_finalize(stmt);
        return -35.0f;  // Return -1 if the record doesn't exist
    }
}

int IpodData::GetMus() {
    const char* query = "SELECT Mus FROM CONFIG WHERE id = 1";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);
    
    if (rc != SQLITE_OK) {
        // If preparing the SELECT statement fails, print the error
        Serial.print("Failed to prepare SELECT statement: ");
        Serial.println(sqlite3_errmsg(db));
        return 1;  // Return -1 to indicate an error
    }

    // Execute the query
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        // If the record with id = 1 is found, return the value of Vol
        int mus = sqlite3_column_double(stmt, 0);  // Assuming Vol is in the first column
        sqlite3_finalize(stmt);  // Clean up the prepared statement
        return mus;
    } else {
        // If no record is found
        Serial.println("Record with ID = 1 not found");
        sqlite3_finalize(stmt);
        return 1;  // Return -1 if the record doesn't exist
    }
}

float IpodData::GetBass() {
    const char* query = "SELECT Bass FROM CONFIG WHERE id = 1";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);
    
    if (rc != SQLITE_OK) {
        // If preparing the SELECT statement fails, print the error
        Serial.print("Failed to prepare SELECT statement: ");
        Serial.println(sqlite3_errmsg(db));
        return 1;  // Return -1 to indicate an error
    }

    // Execute the query
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        // If the record with id = 1 is found, return the value of Vol
        float bass = sqlite3_column_double(stmt, 0);  // Assuming Vol is in the first column
        sqlite3_finalize(stmt);  // Clean up the prepared statement
        return bass;
    } else {
        // If no record is found
        Serial.println("Record with ID = 1 not found");
        sqlite3_finalize(stmt);
        return 1;  // Return -1 if the record doesn't exist
    }
}

float IpodData::GetMiddle() {
    const char* query = "SELECT Middle FROM CONFIG WHERE id = 1";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);
    
    if (rc != SQLITE_OK) {
        // If preparing the SELECT statement fails, print the error
        Serial.print("Failed to prepare SELECT statement: ");
        Serial.println(sqlite3_errmsg(db));
        return 1;  // Return -1 to indicate an error
    }

    // Execute the query
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        // If the record with id = 1 is found, return the value of Vol
        float middle = sqlite3_column_double(stmt, 0);  // Assuming Vol is in the first column
        sqlite3_finalize(stmt);  // Clean up the prepared statement
        return middle;
    } else {
        // If no record is found
        Serial.println("Record with ID = 1 not found");
        sqlite3_finalize(stmt);
        return 1;  // Return -1 if the record doesn't exist
    }
}

float IpodData::GetTreble() {
    const char* query = "SELECT Treble FROM CONFIG WHERE id = 1";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);
    
    if (rc != SQLITE_OK) {
        // If preparing the SELECT statement fails, print the error
        Serial.print("Failed to prepare SELECT statement: ");
        Serial.println(sqlite3_errmsg(db));
        return 1;  // Return -1 to indicate an error
    }

    // Execute the query
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        // If the record with id = 1 is found, return the value of Vol
        float treble = sqlite3_column_double(stmt, 0);  // Assuming Vol is in the first column
        sqlite3_finalize(stmt);  // Clean up the prepared statement
        return treble;
    } else {
        // If no record is found
        Serial.println("Record with ID = 1 not found");
        sqlite3_finalize(stmt);
        return 1;  // Return -1 if the record doesn't exist
    }
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