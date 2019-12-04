#include "inc_dec.h"
#include "../reg/reg.h"
#include <stdbool.h>
#include "../../../../config.h"
#include "../../../../drivers/drivers.h"
#include <stoi.h>
#include <itos.h>

static inline void on_error();

int8_t cmd_inc(char args[], uint8_t args_len) {
    if(args_len != 1) {
        on_error();
        return -1;
    } else {
        inc_reg( stoi(&args[0], 1) );
        return 0;
    }
}

int8_t cmd_dec(char args[], uint8_t args_len) {
    if(args_len != 1) {
        on_error();
        return -1;
    } else {
        dec_reg( stoi(&args[0], 1) );
        return 0;
    }
}

void on_error() {
    Comm_write("Wrong Arguments!", sizeof("Wrong Arguments!") - 1);
}

void inc_reg(uint8_t reg) {
    char data[CMD_REG_LEN];
    uint8_t data_len = getRegData(reg, data, CMD_REG_LEN);
    int32_t num = stoi(data, data_len) + 1;
    data_len = itos(num, data, data_len);
    setRegData(reg, data, data_len);
}

void dec_reg(uint8_t reg) {
    char data[CMD_REG_LEN];
    uint8_t data_len = getRegData(reg, data, CMD_REG_LEN);
    int32_t num = stoi(data, data_len) - 1;
    data_len = itos(num, data, data_len);
    setRegData(reg, data, data_len);
}

