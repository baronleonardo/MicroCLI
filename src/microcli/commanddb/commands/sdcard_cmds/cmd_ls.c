#include "sdcard_cmds.h"
#include "../../../../drivers/drivers.h"
#include <stddef.h>

static int8_t ls(char* path);

int8_t cmd_ls(char args[], uint8_t args_len) {
    if(!SdCard_isInitiated()) 
        return -1;
    else if(args_len == 0)
        return ls("/");
    else if(args[0] != '"') {
        Comm_write("Wrong Arguments", sizeof("Wrong Arguments") - 1);
        Comm_writeNewLine();
        return -1;
    }

    // replace last `"` with `\0`
    args[args_len - 1] = '\0';

    return ls(args + 1);
}

int8_t ls(char* path) {
    File* file = SdCard_open(path);
    if(file == NULL) return -1;

    File* entry = file;
    // TODO: change this number
    uint8_t file_name_len = 30;
    char file_name[file_name_len];

    while( (entry = SdCard_File_openNextFile(file)) != NULL) {
        file_name_len = 30;
        file_name_len = SdCard_File_name(entry, file_name, file_name_len);
        Comm_write(file_name, file_name_len);

        if(SdCard_File_isDirectory(entry)) Comm_writeChar('/');

        Comm_writeNewLine();
        SdCard_File_close(entry);
    }

    SdCard_File_close(file);

    return 0; 
}
