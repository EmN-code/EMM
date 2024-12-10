
#include "contiki.h"
#include "dev/dht11.h"  // Ensure this header file is included
#include <stdio.h>

// Define the GPIO pin for the DHT11 sensor
#define DHT11_GPIO_PIN 2

// Process to read DHT11 sensor data
PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

// Function to read DHT11 sensor data
static void read_dht11_data() {
    int humidity_int, humidity_dec, temperature_int, temperature_dec;
    int status = dht11_read(DHT11_GPIO_PIN, &humidity_int, &humidity_dec, &temperature_int, &temperature_dec);

    if (status == DHT11_STATUS_OKAY) {
        printf("Humidity: %d.%d %%, Temperature: %d.%d C\n", humidity_int, humidity_dec, temperature_int, temperature_dec);
    } else {
        printf("Failed to read DHT11 sensor data, status: %d\n", status);
    }
}

// Process definition
PROCESS_THREAD(dht11_read_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    // Set the timer to trigger every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read DHT11 sensor data
        read_dht11_data();

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main() {
    // Initialize the Contiki system
    contiki_init();

    // Start the DHT11 read process
    process_start(&dht11_read_process, NULL);

    // Enter the Contiki main loop
    while (1) {
        process_run();
    }

    return 0;
}
