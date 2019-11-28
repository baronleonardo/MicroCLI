#include "stoi.h"

int32_t stoi(char* str, uint8_t str_len) {
    // simple optimization
    if(str_len == 1)
        if( (str[0] > '0') && (str[0] < '9') )
            return str[0] - '0';

    int32_t output_int = 0;
    int32_t multiplyer = 1;

    while(str_len--) {
        if( (str[str_len] < '0' || str[str_len] > '9') &&
            (str[str_len] != '-' ) && 
            (str[str_len] != '\0') )
                return -1;

        if( str[str_len] == '-' ) {
            if(output_int) {
                output_int = -output_int;
                break;
            }
        } else {
            output_int += (str[str_len] - '0') * multiplyer;
            multiplyer *= 10;
        }
    }

    return output_int;
}
