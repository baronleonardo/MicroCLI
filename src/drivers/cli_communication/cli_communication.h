#ifndef CLI_COMMUNICATION_H
#define CLI_COMMUNICATION_H


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
void Comm_writeChar(char data);

#ifdef __cplusplus
  }
#endif

#endif // CLI_COMMUNICATION_H
