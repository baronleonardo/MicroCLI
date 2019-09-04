#include "../../../config.h"

#if defined uart && defined esp12e
#include "../../communication.h"
#include "../../../clock/clock.h"
#include "Arduino.h"

extern "C" void Comm_connect() {
    Serial.begin(UART_BAUDRATE);
}

extern "C" bool Comm_isConnected() {
    // FIXME: find a way to make sure serial is connected
    return true;
}

extern "C" char Comm_readChar() {
    static uint32_t cur_time;
    static uint8_t data_recv_len;

    data_recv_len = 0;
    cur_time = Clock_now();
    // check if we received data between time range
    while( ((data_recv_len = Serial.available()) > 0) && 
           ((Clock_now() - cur_time) > MAX_READ_DELAY)
         );

    // timeout
    if( data_recv_len == 0 ) return (char)0;
    else return Serial.read();
}

extern "C" void Comm_write(const char* data, uint32_t len) {
    Serial.write(data, len);
}

extern "C" void Comm_writeChar(char data) {
    Serial.write(data);
}

#endif // uart && esp12e
