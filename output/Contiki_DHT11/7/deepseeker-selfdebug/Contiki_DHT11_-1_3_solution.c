
#include <stdio.h>
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "sys/etimer.h"

// Define the GPIO pin for DHT11
#define DHT11_GPIO_PIN 2

// Function to read DHT11 sensor data
static void read_dht11(void) {
    // Placeholder for DHT11 reading logic
    // This should be replaced with actual DHT11 reading code
    printf("Reading DHT11 sensor data...\n");
}

PROCESS(dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    // Configure the GPIO pin for DHT11
    gpio_hal_pin_configure(DHT11_GPIO_PIN, GPIO_HAL_PIN_MODE_INPUT);

    // Set the timer to trigger every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor
        read_dht11();

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void) {
    // Initialize the Contiki system
    contiki_init();

    // Start the DHT11 sensor process
    process_start(&dht11_process, NULL);

    // Enter the Contiki main loop
    while(1) {
        process_run();
    }

    return 0;
}
