#include "forloop.h"
#include "../reg/reg.h"
#include "../inc_dec/inc_dec.h"
#include <stoi.h>
#include "../../../../config.h"

int8_t forloop(char args[], uint8_t args_len) {
    // return current data in register
    // then decrement it
    if(args_len != 1) return -1;

    uint8_t reg = stoi(&args[0], 1);
    char reg_data_str[CMD_REG_LEN];
    uint32_t reg_data_len = getRegData(reg, reg_data_str, CMD_REG_LEN);
    int32_t reg_data = stoi(reg_data_str, reg_data_len);
    dec_reg(reg);

    return reg_data;
}
