#ifndef CONFIG_H
#define CONFIG_H

/********************************** MCU **********************************/
// available options: esp12e
#define esp12e

/***************************** Communication *****************************/
// available options: uart
#define uart
#define UART_BAUDRATE   115200
#define MAX_READ_DELAY  200          // in microseconds

/******************************* MicroCMD *******************************/
#define CMD_INPUT_MAX_LEN       101
// 1 -> '\n' 
// 2 -> '\r' 
// 3 -> "\r\n"
#define CMD_LINE_ENDING         3
#define CMD_NEW_CMD_INDICATOR   "> "
#define CMD_NAME_LEN            11
#define CMD_ARGS_COUNT          5
#define CMD_REG_LEN             11

#endif // CONFIG_H
