#include "../../../config.h"
#ifdef sdcard

#include "../sdcard.h"
#include "Arduino.h"
#include <SD.h>
#include <cstring>

static bool isInitiated = false;

extern "C" void SdCard_init() {
    isInitiated = SD.begin(SDCARD_CS_PIN);
}

extern "C" void SdCard_deinit() {
    isInitiated = false;
    SD.end();
}

extern "C" bool SdCard_isInitiated() {
    return isInitiated;
}

extern "C" File* SdCard_open(const char* path) {
    File* file = new File;
    *file = SD.open(path);

    if(*file) 
        return file;
    else {
        delete file;
        return NULL;
    }
}

extern "C" bool SdCard_isPathExists(const char* path) {
   return SD.exists(path);
}

extern "C" uint8_t SdCard_File_name(File* file, char* name, uint8_t name_len) {
    String f_name = file->name();
    if(f_name.length() > name_len)
        return 0;
    strncpy(name, f_name.c_str(), name_len);
    return f_name.length();
}

extern "C" int32_t SdCard_File_availableDataSize(File* file) {
    return file->available();
}

extern "C" void SdCard_File_close(File* file) {
    file->close();
    delete file;
}

extern "C" char SdCard_File_peek(File* file) {
    return file->peek();
}

extern "C" uint32_t SdCard_File_currentPosition(File* file) {
    return file->position();
}

extern "C" bool SdCard_File_seek(File* file, uint32_t pos) {
    return file->seek(pos);
}

extern "C" uint32_t SdCard_File_size(File* file) {
    return file->size();
}

extern "C" int32_t SdCard_File_read(File* file, char* buf, uint16_t buf_len) {
    return file->readBytes(buf, buf_len);
}

extern "C" int32_t SdCard_File_readLine(File* file, char* buf, uint16_t buf_len) {
    // TODO: change '\n'
    return file->readBytesUntil('\n', (uint8_t*)buf, buf_len);
}

extern "C" char SdCard_File_readChar(File* file) {
    return file->read();
}

extern "C" bool SdCard_File_isDirectory(File* file) {
    return file->isDirectory();
}

extern "C" File* SdCard_File_openNextFile(File* file) {
    File* next_file = new File;
    *next_file = file->openNextFile();

    if(*next_file)
        return next_file;
    else {
        delete next_file;
        return NULL;
    }
}

extern "C" void SdCard_File_rewindDirector(File* file) {
    file->rewindDirectory();
}

#endif // sdcard
