#include "reg.h"
#include "../../../../config.h"
#include <string.h>
#include "../../../../drivers/drivers.h"

typedef struct Register {
    char data[CMD_REG_LEN];
    uint8_t data_len;
} Register;

Register regs[CMD_REGS_COUNT];

void regSet(char args[], uint8_t args_len) {
    if( (args[0] != '"') && ((args[1] < '0') || (args[1] > '9')) ) {
        Comm_write("Wrong Arguments", sizeof("Wrong Arguments") - 1);
        Comm_writeNewLine();
    }

    // 3 here represent the single/double quotations
    // and first element in the args which represent
    // register number
    uint8_t reg_index = args[1] - '0';
    setRegData(reg_index, &args[2], args_len - 3);
}

void regGet(char args[], uint8_t args_len) {
    if( (args[0] != '"') && ((args[1] < '0') || (args[1] > '9')) ) {
        Comm_write("Wrong Arguments", sizeof("Wrong Arguments") - 1);
        Comm_writeNewLine();
    }

    Register* reg = &regs[args[1] - '0'];
    Comm_write("Register #", sizeof("Register #") - 1);
    Comm_writeChar(args[1]);
    Comm_write(": ", sizeof(": ") - 1);
    Comm_write(reg->data, reg->data_len);
    Comm_writeNewLine();
}

bool setRegData(uint8_t reg, char* data, uint8_t data_len) {
    if(reg >= CMD_REGS_COUNT) return false;

    strncpy(regs[reg].data, data, data_len);
    regs[reg].data_len = data_len;

    return true;
}

uint8_t getRegData(uint8_t reg, char* data, uint8_t data_len) {
    if(reg >= CMD_REGS_COUNT) return 0;
    uint8_t len = data_len < regs[reg].data_len ? data_len : regs[reg].data_len;
    strncpy(data, regs[reg].data, len);

    return len;
}
