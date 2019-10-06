#include "../gpio.h"
#include "Arduino.h"

extern "C" void gpio_init() {
}

extern "C" void gpio_setMode(uint8_t pin_num, GPIO_MODE mode) {
    // FIXME: better idea than just retrun
    if(mode > GPIO_MODE_INPUTPULLUP) return;
    pinMode(pin_num, (int)mode);
}

extern "C"  void gpio_setState(uint8_t pin_num, GPIO_STATE state) {
    // FIXME: better idea than just retrun
    if(state > GPIO_STATE_HIGH) return;
    digitalWrite(pin_num, (int)state);
}

extern "C" GPIO_STATE gpio_getState(uint8_t pin_num) {
    return (GPIO_STATE)digitalRead(pin_num);
}
