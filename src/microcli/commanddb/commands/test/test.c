#include "test.h"
#include "../../../../drivers/drivers.h"

int8_t cmd_test(char args[], uint8_t args_len) {
    if(args_len == 0) {
        Comm_write( "Test is done", 
                    sizeof("Test is done") - 1 );
        Comm_writeNewLine();
        return 0;
    } else {
        Comm_write( "This command takes no args!", 
                    sizeof("This command takes no args!") - 1 );
        Comm_writeNewLine();
        return -1;
    }
}
