#include "contiki.h"
#include "dev/gpio-hal.h"
#include <stdio.h>

// Mock function definitions for DHT11 sensor reading
int dht11_read_temp_int(void) {
    return 25; // Mock temperature integer value
}

int dht11_read_humidity_int(void) {
    return 60; // Mock humidity integer value
}

PROCESS(read_dht11_process, "Read DHT11 Sensor Process");
AUTOSTART_PROCESSES(&read_dht11_process);

PROCESS_THREAD(read_dht11_process, ev, data) {
    static struct etimer timer;
    PROCESS_BEGIN();

    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        
        int temperature = dht11_read_temp_int();
        int humidity = dht11_read_humidity_int();

        printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);

        etimer_reset(&timer);
    }

    PROCESS_END();
}