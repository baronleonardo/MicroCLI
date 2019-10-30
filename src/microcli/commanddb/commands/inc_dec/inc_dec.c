#include "inc_dec.h"
#include "../reg/reg.h"
#include <stdbool.h>
#include "../../../../config.h"
#include "../../../../drivers/drivers.h"
#include <atoi.h>
#include <itoa.h>

static inline void on_error();

void cmd_inc(char args[], uint8_t args_len) {
    if(args_len != 1)
        on_error();

    else
        inc_reg( atoi(&args[0], 1) );
}

void cmd_dec(char args[], uint8_t args_len) {
    if(args_len != 1)
        on_error();

    else
        dec_reg( atoi(&args[0], 1) );
}

void on_error() {
    Comm_write("Wrong Arguments!", sizeof("Wrong Arguments!") - 1);
}

void inc_reg(uint8_t reg) {
    char data[CMD_REG_LEN];
    uint8_t data_len = getRegData(reg, data, CMD_REG_LEN);
    int32_t num = atoi(data, data_len) + 1;
    data_len = itoa(num, data, data_len);
    setRegData(reg, data, data_len);
}

void dec_reg(uint8_t reg) {
    char data[CMD_REG_LEN];
    uint8_t data_len = getRegData(reg, data, CMD_REG_LEN);
    int32_t num = atoi(data, data_len) - 1;
    data_len = itoa(num, data, data_len);
    setRegData(reg, data, data_len);
}

