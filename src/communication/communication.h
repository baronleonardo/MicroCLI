#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdint.h>

void Comm_connect();
bool Comm_isConnected();
// delim: this param will be passed if delim=(char)0
// max_time: this param will be passed if max_time=0
uint32_t Comm_read(char* data, 
                   uint32_t len,
                   char delim);
void Comm_write(char* data, uint32_t len);

#endif // COMMUNICATION_H
