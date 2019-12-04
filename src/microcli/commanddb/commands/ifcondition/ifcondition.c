#include "ifcondition.h"
#include <stdbool.h>
#include "../../../../config.h"
#include "../../../../drivers/drivers.h"
#include <stddef.h>

static bool is_conditional_operator(char op);
static inline void printError();
static int exec(int32_t operand1, char operator[], uint8_t operator_len, int32_t operand2);
static int32_t get_reg_data_int(char reg_str);

int8_t cmd_if(char args[], uint8_t args_len) {
    if(args_len == 0) {
        printError();
        return -1;
    }

    uint8_t counter = 0;
    while(args[counter] < '0') counter++;
 
    int32_t reg1_data = get_reg_data_int(args[counter]);

    if(args_len == 1) {
        if( reg1_data > 0 )
            return 1;
    } else {
        char operator[2] = { 0 };
        while(args[++counter] <= ' ');
        operator[0] = args[counter];

        if( is_conditional_operator(args[counter + 1]) )
            operator[1] = args[++counter];

        while(args[++counter] <= ' ');

        int32_t reg2_data = get_reg_data_int(args[counter]);
    
        uint8_t exec_status = exec(reg1_data, operator, operator[1] == 0 ? 1 : 2, reg2_data);

        if(exec_status == -1)
            printError();

        return exec_status;
    }

    return 0;
}

int32_t get_reg_data_int(char reg_str) {
    // TODO: this function should be part of `reg` command
    uint8_t reg = stoi(&reg_str, 1);
    char reg_data_str[CMD_REG_LEN];
    uint32_t reg_data_len = getRegData(reg, reg_data_str, CMD_REG_LEN);
    int32_t reg_data = stoi(reg_data_str, reg_data_len);

    return reg_data;
}

bool is_conditional_operator(char op) {
    const char operators[] = { '!', '<', '=', '>' };

    for(uint8_t iii = 0; iii < sizeof(operators); ++iii) {
        if(op == operators[iii])
            return true;
    }

    return false;
}

int exec(int32_t operand1, char* operator, uint8_t operator_len, int32_t operand2) {
    if(operator_len == 1) {
        if(operator[0] == '<') {
            if(operand1 < operand2)
                return 1;
        } else if(operator[0] == '>') {
            if(operand1 > operand2)
                return 1;
        } else {
            return -1;
        }
    } else {
        if(operator[1] != '=')
            return -1;

        if(operator[0] == '!') {
            if(operand1 != operand2)
                return 1;
        } else if(operator[0] == '=') {
            if(operand1 == operand2)
                return 1;
        } else if(operator[0] == '>') {
            if(operand1 >= operand2)
                return 1;
        } else if(operator[0] == '<') {
            if(operand1 <= operand2)
                return 1;
        } else {
            return -1;
        }
    }

    return 0;
}

void printError() {
    Comm_write("Wrong Arguments", sizeof("Wrong Arguments") - 1);
    Comm_writeNewLine();
}
