#ifndef CONFIG_H
#define CONFIG_H

/********************************** MCU **********************************/
// available options: esp12e
#define esp12e

/***************************** Communication *****************************/
// available options: uart
#define uart
#define UART_BAUDRATE 115200
#define MAX_READ_DELAY 500          // in microseconds

#endif // CONFIG_H
