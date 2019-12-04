#ifndef REG_H
#define REG_H

#include <stdint.h>
#include <stdbool.h>

// commands
int8_t regSet(char args[], uint8_t args_len);
int8_t regGet(char args[], uint8_t args_len);

// callable functions
bool setRegData(uint8_t reg, char* data, uint8_t data_len);
uint8_t getRegData(uint8_t reg, char* data, uint8_t data_len);

#endif // REG_H
