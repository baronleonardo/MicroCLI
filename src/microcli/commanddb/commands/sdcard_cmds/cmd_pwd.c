//#include "sdcard_cmds.h"
//#include "../../../../drivers/drivers.h"
//#include <stddef.h>
//
//static uint8_t get_currentDirectory(File* cur_file, char* file_name, uint8_t max_file_name_len);
//
//int8_t cmd_pwd(char args[], uint8_t args_len) {
//    File* file = SdCard_getCurrentFile();
//    if(file == NULL) {
//        Comm_writeChar('-');
//        return -1;
//    }
//    else {
//        // TODO: max len
//        uint8_t file_name_len = 100;
//        char file_name[file_name_len];
//
//        file_name_len = get_currentDirectory(file, file_name, file_name_len);
//
//        Comm_write(file_name, file_name_len);
//        Comm_writeNewLine();
//    }
//
//    return 0;
//}
//
//uint8_t get_currentDirectory(File* cur_file, char* file_name, uint8_t max_file_name_len) {
//    uint8_t file_name_len = SdCard_File_name(cur_file, file_name, max_file_name_len);
//    return file_name_len;
//
//    if(SdCard_File_isDirectory(cur_file))
//        return file_name_len;
//        
//    else {
//        uint8_t counter = 0;
//        for(uint8_t iii = file_name_len - 1; iii >= 0; iii--) {
//            if(file_name[iii] == '/') break;
//            counter++;
//        }
//
//        if(file_name_len - counter == 0) {
//            file_name[0] = '/';
//            return 1;
//        } else
//            return file_name_len - counter;
//    }
//}
