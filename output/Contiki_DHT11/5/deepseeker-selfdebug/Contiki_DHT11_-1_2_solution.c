
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "sys/etimer.h"
#include <stdio.h>

// Define the GPIO pin for DHT11
#define DHT11_GPIO_PIN 2

// Function to read DHT11 sensor data
static void read_dht11(void) {
    // Placeholder for DHT11 reading logic
    // This should be replaced with actual DHT11 reading code
    printf("Reading DHT11 sensor data...\n");
}

PROCESS(dht11_reading_process, "DHT11 Reading Process");
AUTOSTART_PROCESSES(&dht11_reading_process);

PROCESS_THREAD(dht11_reading_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    // Configure GPIO pin for DHT11
    gpio_hal_pin_set_output(DHT11_GPIO_PIN);

    // Set timer for 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read DHT11 sensor data
        read_dht11();

        // Reset timer for the next reading
        etimer_reset(&timer);
    }

    PROCESS_END();
}
