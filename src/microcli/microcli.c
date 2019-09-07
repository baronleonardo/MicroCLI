#include "microcli.h"
#include "../mcu/mcu.h"
#include "../communication/communication.h"
#include "../config.h"
#include "command.h"
#include <stdbool.h>
#include <stdint.h>

/***************************** Static Variables *****************************/
const char newCmdIndicator[]        = "> ";
const uint8_t newCmdIndicatorLen    = 2;
char cmd_raw_input[CMD_INPUT_MAX_LEN];
uint32_t current_input_length       = 0;
/****************************************************************************/

/********************************** Macros **********************************/
#define NUL         ((char)0)
#define EOT         ((char)4)
#define BACKSPACE   ((char)8)
/****************************************************************************/

/************************** Functions declaration **************************/
inline void __Microcli_sendNewCmdIndicator();
inline void __Microcli_onOverflowInputLength();
inline void __Microcli_onDelimiterKeyReceived();
inline void __Microcli_onEndOfTransmitionKeyReceived();
inline void __Microcli_onBackspaceKeyReceived();
/***************************************************************************/

void Microcli_init() {
    Mcu_init();

    Comm_connect();
    // wait for a connection
    while( !Comm_isConnected() )
        Mcu_hookAfterEachCycle();
}

void Microcli_mainLoop() {
    __Microcli_sendNewCmdIndicator();

    while(true) {
        cmd_raw_input[current_input_length] = Comm_readChar();

        if(cmd_raw_input[current_input_length] != NUL) {

            if(current_input_length == CMD_INPUT_MAX_LEN)
                __Microcli_onOverflowInputLength();

            else {
                Comm_writeChar(cmd_raw_input[current_input_length]);

                switch( cmd_raw_input[current_input_length] ) {
                    case CMD_DELIMITER:
                        __Microcli_onDelimiterKeyReceived();
                        break;

                    case EOT:
                        __Microcli_onEndOfTransmitionKeyReceived();
                        break;

                    case BACKSPACE:
                        __Microcli_onBackspaceKeyReceived();
                        break;

                    default:
                        current_input_length++;
                }
            }
        }

        Mcu_hookAfterEachCycle();
    }
}

/***************************** Private functions *****************************/
void __Microcli_sendNewCmdIndicator() {
    Comm_write(newCmdIndicator, newCmdIndicatorLen);
}

void __Microcli_onOverflowInputLength() {
    const char overflow_msg[] = "\rYou exceeded the max number of charaters for one command! reseting\r";
    const uint8_t overflow_msg_len = sizeof(overflow_msg);
    Comm_write(overflow_msg, overflow_msg_len);
    __Microcli_sendNewCmdIndicator();
    current_input_length = 0;
}

void __Microcli_onDelimiterKeyReceived() {
    cmd_raw_input[current_input_length] = '\0';

    Command_parse(cmd_raw_input);

    __Microcli_sendNewCmdIndicator();
    current_input_length = 0;
}

void __Microcli_onEndOfTransmitionKeyReceived() {
    // on Linux   -> <Ctrl+d>
    // on Windows -> <Ctrl+z>
    Comm_writeChar(CMD_DELIMITER);
    __Microcli_sendNewCmdIndicator();
    current_input_length = 0;
}

void __Microcli_onBackspaceKeyReceived() {
    current_input_length--;
}
/*****************************************************************************/