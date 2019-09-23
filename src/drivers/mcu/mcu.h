#ifndef MCU_H
#define MCU_H

void Mcu_init();
// This function is called at the cycle end of `loop()` function 
// or at the cycle end of any non-return function
void Mcu_HookAfterEachCycle();

#endif // MCU_H
