#include <stdlib.h>
#include <stdint.h>
#include "contiki.h"

PROCESS(dht11_rgb_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_process);

static struct etimer timer;

void read_dht11_and_control_led() {
    // Simulating a delay that fits within uint16_t range
    etimer_set(&timer, CLOCK_SECOND * 5u); // 5 seconds
    // Placeholders for reading the DHT11 and controlling RGB LED
    printf("Reading DHT11 sensor\n");
    printf("Turning on RGB LED\n");
}

PROCESS_THREAD(dht11_rgb_process, ev, data) {
    PROCESS_BEGIN();

    while(1) {
        read_dht11_and_control_led();

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    }

    PROCESS_END();
}
