#ifndef CONFIG_H
#define CONFIG_H

/********************************** MCU **********************************/
// available options: esp12e
#define esp12e

/***************************** Communication *****************************/
// available options: uart
#define uart
#define UART_BAUDRATE 115200
#define MAX_READ_DELAY 200          // in microseconds

/******************************* MicroCMD *******************************/
#define CMD_INPUT_MAX_LEN   101
#define CMD_DELIMITER       '\r'

#endif // CONFIG_H
