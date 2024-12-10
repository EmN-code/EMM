
#include "contiki.h"
#include "dev/gpio.h"
#include "sys/etimer.h"
#include <stdio.h>

// Define the GPIO pin for DHT11
#define DHT11_PIN 2

// Function to read DHT11 sensor data
void read_dht11(void) {
    // Placeholder for DHT11 reading logic
    // This should be replaced with actual DHT11 reading code
    printf("DHT11 data read\n");
}

PROCESS(dht11_reading_process, "DHT11 Reading Process");
AUTOSTART_PROCESSES(&dht11_reading_process);

PROCESS_THREAD(dht11_reading_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    // Set the timer to trigger every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read DHT11 sensor data
        read_dht11();

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void) {
    // Initialize the Contiki system
    process_init();
    process_start(&etimer_process, NULL);
    process_start(&dht11_reading_process, NULL);

    // Main loop
    while(1) {
        process_run();
    }

    return 0;
}
