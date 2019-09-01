#include "../../../config.h"

#if defined uart && defined esp12e
#include "../../communication.h"
#include "../../../clock/clock.h"
#include "Arduino.h"

void Comm_connect() {
    Serial.begin(UART_BAUDRATE);
}

bool Comm_isConnected() {
    // FIXME: find a way to make sure serial is connected
    return true;
}

uint32_t Comm_read(char* data, 
                   uint32_t len,
                   char delim) {
    uint32_t cur_time;
    uint8_t data_recv_len;
    uint32_t counter;

    for( counter = 0; counter < len - 1; ++counter ) {
        data_recv_len = 0;
        cur_time = Clock_now();
        while( ((data_recv_len = Serial.available()) > 0) && ((Clock_now() - cur_time) > MAX_READ_DELAY) );

        // timeout
        if( data_recv_len == 0 ) break;

        data[counter] = Serial.read();
        if( data[counter] == delim ) break;
    }

    data[counter] = '\0';
    return counter;
}

void Comm_write(char* data, uint32_t len) {
    Serial.write(data, len);
}

#endif // uart && esp12e
