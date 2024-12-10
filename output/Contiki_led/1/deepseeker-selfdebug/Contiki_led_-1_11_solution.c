
#include "contiki.h"
#include "dev/dht11-sensor.h" // Ensure this header is available
#include "dev/leds.h"
#include <stdio.h>

// Define the period for reading the DHT11 sensor
#define READ_PERIOD (5 * CLOCK_SECOND)

// Function to read DHT11 sensor data
static void read_dht11(void) {
    int temperature, humidity;
    if (dht11_read(&temperature, &humidity) == DHT11_STATUS_OKAY) {
        printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
        // Turn on the RGB LED
        leds_set(LEDS_RED); // Example: Turn on the red LED
    } else {
        printf("Failed to read DHT11 sensor\n");
    }
}

// Process to handle periodic reading of the DHT11 sensor
PROCESS(dht11_reader_process, "DHT11 Reader Process");
AUTOSTART_PROCESSES(&dht11_reader_process);

PROCESS_THREAD(dht11_reader_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    // Initialize the DHT11 sensor
    dht11_init();

    // Set the timer for periodic reading
    etimer_set(&timer, READ_PERIOD);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor
        read_dht11();

        // Reset the timer for the next reading
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void) {
    // Start the Contiki system
    contiki_init();

    // Start the DHT11 reader process
    process_start(&dht11_reader_process, NULL);

    // Enter the Contiki main loop
    while (1) {
        process_run();
    }

    return 0;
}
