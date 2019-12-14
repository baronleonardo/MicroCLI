#ifndef SDCARD_CMDS_H
#define SDCARD_CMDS_H

#include <stdint.h>

int8_t cmd_ls(char args[], uint8_t args_len);
//int8_t cmd_cd(char args[], uint8_t args_len);
//int8_t cmd_pwd(char args[], uint8_t args_len);
int8_t cmd_cat(char args[], uint8_t args_len);
int8_t cmd_mount(char args[], uint8_t args_len);
int8_t cmd_umount(char args[], uint8_t args_len);

#endif // SDCARD_CMDS_H
