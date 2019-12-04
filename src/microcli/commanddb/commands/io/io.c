#include "io.h"
#include "../../../../drivers/drivers.h"
#include "../reg/reg.h"
#include <stoi.h>
#include <stdbool.h>

inline static void __on_error();
inline static bool  __print_pin_state(char pin_s);

int8_t cmd_io(char args[], uint8_t args_len) {
    Comm_write(args, args_len);
    Comm_writeNewLine();
    // if no arguments but a register, then we need to read pin state
    if(args_len == 1) {
        if(__print_pin_state(args[0]))
            return 0;
        else
            return -1;
    }

    char pin_s       = (char)0;
    GPIO_MODE mode   = -1;
    GPIO_STATE state = -1;

    for(uint8_t iii = 0; iii < args_len; ++iii) {
        if( args[iii] == 'h' )
            state = GPIO_STATE_HIGH;
        else if( args[iii] == 'l' )
            state = GPIO_STATE_LOW;
        else if( (args[iii] >= '0') && (args[iii] <= '9') )
            getRegData( stoi(&args[iii], 1), &pin_s, 1 );
        else if( args[iii] == 'i' )
            mode = GPIO_MODE_INPUT;
        else if( args[iii] == 'o' )
            mode = GPIO_MODE_OUTPUT;
        else if( args[iii] == 'p' )
            mode = GPIO_MODE_INPUTPULLUP;
        else {
            __on_error();
            return -1;
        }
    }

    if( (pin_s == (char)0) || (pin_s < '0') || (pin_s > '9') ) {
        __on_error();
        return -1;
    }

    uint8_t pin_num = stoi(&pin_s, 1);
    if(mode != -1)
        gpio_setMode(pin_num, mode);

    if( (mode == -1) || (mode == GPIO_MODE_OUTPUT) )
        gpio_setState(pin_num, state);

    return 0;
}

void __on_error() {
    Comm_write("Wrong arguments!", sizeof("Wrong arguments!") - 1);
    Comm_writeNewLine();
}

bool __print_pin_state(char pin) {
    if( (pin >= '0') && (pin <= '9') ) {
        Comm_write("Pin #", sizeof("Pin #") - 1);
        Comm_writeChar(pin);
        Comm_write(" state: ", sizeof(" state: ") - 1);
        if( gpio_getState(stoi(&pin, 1)) == GPIO_STATE_HIGH )
            Comm_write("high", sizeof("high") - 1);
        else
            Comm_write("low", sizeof("low") - 1);
        Comm_writeNewLine();
        return true;
    } else { 
        __on_error();
        return false;
    }
}
