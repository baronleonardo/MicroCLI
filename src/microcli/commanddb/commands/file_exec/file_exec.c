#include "file_exec.h"
#include "../../../microcli_engine.h"
#include "../../../../config.h"
#include "../../../../drivers/drivers.h"
#include <stddef.h>

int8_t file_exec(char args[], uint8_t args_len) {
    Comm_write(args, args_len);
    Comm_writeNewLine();
    if(args_len == 0) {
        // TODO: ERROR
        return -1;
    } 
    else if( (args[0] != '\"') && (args[0] != '\'') ) {
        // TODO: ERROR
        return -1;
    } else if( (args[args_len - 1] != '\"') && (args[args_len - 1] != '\'') ) {
        // TODO: ERROR
        return -1;
    }

    args[args_len - 1] = '\0';
    File* file = SdCard_open(args + 1);

    if(file == NULL) {
        // ERROR
        return -1;
    }

    char cur_line_input[CMD_INPUT_MAX_LEN];
    int32_t input_len = 0;
    Command* cmd = NULL;

    while(SdCard_File_availableDataSize(file) > 0) {
        input_len = SdCard_File_readLine(file, cur_line_input, CMD_INPUT_MAX_LEN);
        // TODO: remove this part
        cur_line_input[input_len++] = '\0';
        cmd = Command_parse(cur_line_input, input_len);
        if(cmd != NULL) Command_exec(cmd);
    }

    return 0;
}
