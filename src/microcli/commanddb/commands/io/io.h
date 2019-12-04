#ifndef IO_H
#define IO_H

#include <stdint.h>

/* This control pin state/mode
 * mode  -> input, output, input pullup
 * state -> high, low
 * Args:
 *  -h: high         (mode)  (optional)
 *  -l: low          (mode)  (optional)
 *  -i: input        (state) (optional)
 *  -o: output       (state) (optional)
 *  -p: input pullup (state) (optional)
 * reg: pin number           (must)
 */
int8_t cmd_io(char args[], uint8_t args_len);

#endif // IO_H
