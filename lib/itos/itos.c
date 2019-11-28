#include "itos.h"
#include <stdbool.h>

void reverse_str(char* str, uint8_t str_len) {
    if(str_len == 1) return;

    char temp;
    for(uint8_t iii = 0, jjj = str_len - 1; iii < jjj; iii++, jjj--) {
        temp = str[iii];
        str[iii] = str[jjj];
        str[jjj] = temp;
    }
}

uint8_t itos(int32_t num, char* str, uint8_t str_len) {

    // simple optimization
    // if there is only one digit
    if( (num >= 0) && (num <= 9) ) {
        str[0] = '0' + num;
        return 1;
    }

    uint8_t counter = 0;
    uint32_t digit;
    bool is_negative = false;

    if(num < 0) {
        is_negative = true;
        num = -num;
    } else if(num == 0)
        str[counter++] = '0';

    while(num != 0) {
        digit = num % 10;
        str[counter++] = digit + '0';
        num /= 10;
    }

    if(is_negative) str[counter++] = '-';

    str[counter] = '\0';
    reverse_str(str, counter);

    return counter;
}
