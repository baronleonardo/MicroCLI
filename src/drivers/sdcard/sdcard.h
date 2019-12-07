#ifndef SDCARD_H
#define SDCARD_H

#ifdef sdcard

#ifdef __cplusplus
  #include <cstdint>
  #include <SD.h>
  extern "C" {
#else
  #include <stdint.h>
  #include <stdbool.h>

  typedef struct File File;
#endif

void SdCard_init();
void SdCard_deinit();
bool SdCard_isInitiated();
// 0-terminated string path
// open for read
File SdCard_open(const char* path);
bool SdCard_isPathExists(const char* path);
uint8_t SdCard_File_name(File* file, char* name, uint8_t name_len);
int32_t SdCard_File_availableDataSize(File* file);
void SdCard_File_close(File* file);
char SdCard_File_peek(File* file);
uint32_t SdCard_File_currentPosition(File* file);
bool SdCard_File_seek(File* file, uint32_t pos);
uint32_t SdCard_File_size(File* file);
int32_t SdCard_File_read(File* file, char* buf, uint16_t buf_len);
int32_t SdCard_File_readLine(File* file, char* buf, uint16_t buf_len);
char SdCard_File_readChar(File* file);
bool SdCard_File_isDirectory(File* file);
File SdCard_File_openNextFile(File* file);
void SdCard_File_rewindDirector(File* file);

#ifdef __cplusplus
  }
#endif

#endif // sdcard

#endif // SDCARD_H
