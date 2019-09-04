#include "Arduino.h"
extern "C" {
    #include "microcli/microcli.h"
}

void setup() {
    Microcli_init();
}

void loop() {
    Microcli_mainLoop();
}
