#include "sdcard_cmds.h"
#include "../../../../drivers/drivers.h"
#include "../../../../config.h"
#include <stddef.h>

int8_t cmd_cat(char args[], uint8_t args_len) {
    if(!SdCard_isInitiated()) return -1;
    else if(args[0] != '"') {
        Comm_write("Wrong Arguments", sizeof("Wrong Arguments") - 1);
        Comm_writeNewLine();
        return -1;
    }

    // replace last `"` with `\0`
    args[args_len - 1] = '\0';

    File* file = SdCard_open(args + 1);

    if(file == NULL) {
        Comm_write("Wrong path", sizeof("Wrong path") - 1);
        Comm_writeNewLine();
        return -1;
    }

    char line[CMD_INPUT_MAX_LEN];
    int32_t line_len;

    while(SdCard_File_availableDataSize(file) > 0) {
        line_len = SdCard_File_readLine(file, line, CMD_INPUT_MAX_LEN);
        Comm_write(line, line_len);
        Comm_writeNewLine();
    }

    SdCard_File_close(file);

    return 0;
}
