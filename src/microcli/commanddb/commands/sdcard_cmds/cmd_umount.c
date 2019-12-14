#include "sdcard_cmds.h"
#include "../../../../drivers/drivers.h"

int8_t cmd_umount(char args[], uint8_t args_len) {
    if(!SdCard_isInitiated()) return -1;
    else if(args_len != 0) {
        Comm_write("Wrong Arguments", sizeof("Wrong Arguments") - 1);
        Comm_writeNewLine();
        return -1;
    }

    SdCard_deinit();

    return 0;
}
