#ifndef GPIO_H
#define GPIO_H

#ifdef __cplusplus
  #include <cstdint>
  extern "C" {
#else
  #include <stdint.h>
  #include <stdbool.h>
#endif

typedef enum GPIO_MODE {
    GPIO_MODE_INPUT,
    GPIO_MODE_OUTPUT,
    GPIO_MODE_INPUTPULLUP,
} GPIO_MODE;

typedef enum GPIO_STATE {
    GPIO_STATE_LOW,
    GPIO_STATE_HIGH,
} GPIO_STATE;

void gpio_init();
void gpio_setMode(uint8_t pin_num, GPIO_MODE mode);
void gpio_setState(uint8_t pin_num, GPIO_STATE state);
GPIO_STATE gpio_getState(uint8_t pin_num);

#ifdef __cplusplus
  }
#endif

#endif // GPIO_H
