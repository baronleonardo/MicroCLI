#include "test.h"
#include "../../../../drivers/drivers.h"

void cmd_test(char args[], uint8_t args_len) {
    const char out[] = "Test is done";
    Comm_write(out, sizeof(out) - 1);
}
