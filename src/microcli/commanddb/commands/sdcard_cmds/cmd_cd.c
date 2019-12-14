//#include "sdcard_cmds.h"
//#include "../../../../drivers/drivers.h"
//#include <stddef.h>
//
//int8_t cmd_cd(char args[], uint8_t args_len) {
//    if(!SdCard_isInitiated()) return -1;
//    if(args[0] != '"') {
//        Comm_write("Wrong Arguments", sizeof("Wrong Arguments") - 1);
//        Comm_writeNewLine();
//        return -1;
//    }
//
//    // replace last `"` with `\0`
//    args[args_len - 1] = '\0';
//
//    File* file = SdCard_open(args + 1);
//
//    if(file == NULL) {
//        Comm_write("Wrong path", sizeof("Wrong path") - 1);
//        Comm_writeNewLine();
//        return -1;
//    } else if(!SdCard_File_isDirectory(file)) {
//        Comm_write("This is not a directory", sizeof("This is not a directory") - 1);
//        Comm_writeNewLine();
//        return -1;
//    }
//
//    return 0;
//}
