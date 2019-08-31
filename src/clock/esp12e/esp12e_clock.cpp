#include "../../config.h"

#ifdef esp12e
#include "../clock.h"
#include "Arduino.h"

void Clock_init() {
}

void Clock_startTicking() {
}

unsigned long Clock_now() {
    return micros();
}

#endif // esp12e
