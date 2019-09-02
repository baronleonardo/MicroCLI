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
char Comm_readChar();
void Comm_write(const char* data, uint32_t len);


#ifdef __cplusplus
  }
#endif

#endif // COMMUNICATION_H
