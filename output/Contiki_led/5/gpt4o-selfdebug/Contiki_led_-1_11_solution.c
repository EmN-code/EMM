#include "sys/etimer.h"
#include "sys/ctimer.h"
#include "contiki.h"
#include <stdio.h>

PROCESS(read_sensor_process, "DHT11 Read and RGB LED Process");
AUTOSTART_PROCESSES(&read_sensor_process);

static struct etimer timer;

void read_dht11() {
    printf("Reading DHT11...");
    // Here should go the real reading logic
    printf("DHT11 data read successful.\n");
}

void turn_on_rgb_led() {
    printf("Turning on RGB LED.");
    // Simulate turning on RGB LED
}

PROCESS_THREAD(read_sensor_process, ev, data) {
    PROCESS_BEGIN();
    
    // Set the timer interval to 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        
        read_dht11();
        turn_on_rgb_led();

        // Reset the timer again
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main() {
    process_init();
    process_start(&read_sensor_process, NULL);
    while(1) {
        process_run();
    }
    return 0;
}