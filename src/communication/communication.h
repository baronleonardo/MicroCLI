#ifndef COMMUNICATION_H
#define COMMUNICATION_H


#ifdef __cplusplus
  #include <cstdint>
  extern "C" {
#else
  #include <stdint.h>
  #include <stdbool.h>
#endif

void Comm_connect();
bool Comm_isConnected();
// delim: this param will be passed if delim=(char)0
// max_time: this param will be passed if max_time=0
uint32_t Comm_read(char* data, 
                   uint32_t len,
                   char delim);
void Comm_write(char* data, uint32_t len);


#ifdef __cplusplus
  }
#endif

#endif // COMMUNICATION_H
