#ifndef INC_DEC_H
#define INC_DEC_H

#include <stdint.h>

void cmd_inc(char args[], uint8_t args_len);
void cmd_dec(char args[], uint8_t args_len);
void inc_reg(uint8_t reg);
void dec_reg(uint8_t reg);

#endif // INC_DEC_H
