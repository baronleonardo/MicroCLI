#include "../../../config.h"

#ifdef esp12e
#include "Arduino.h"

void Mcu_init() {
}

void Mcu_hookAfterEachCycle() {
    // this must be called or watchdog will be fired
    yield();
}

#endif // esp12e
