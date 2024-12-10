#include "contiki-default-conf.h" // Including the default configuration for Contiki
#include <stdio.h> // Standard input/output for logging
#include "coap-blocking-api.h" // Include COAP blocking API

// Declare the sensor and other peripherals for NRF52840
extern const struct sensors_sensor dht11_sensor;

// Mock functions to simulate hardware behavior
#define RGB_LED_ON() printf("RGB LED is turned on.\n")

PROCESS(dht11_read_process, "DHT11 Sensor Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data) {
    static struct etimer et;

    PROCESS_BEGIN();

    // Set a timer to trigger every 5 seconds
    etimer_set(&et, CLOCK_SECOND * 5);

    while (1) {
        // Wait for the timer event
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Simulate reading the sensor
        printf("Reading DHT11 sensor...\n");
        // Check if the sensor is ready
        int result = dht11_sensor.value(0);
        if (result != -1) {
            printf("Temperature: %d\n", result);
            RGB_LED_ON(); // Turn on the RGB LED after a successful read
        } else {
            printf("Failed to read from the DHT11 sensor.\n");
        }

        // Reset the timer for the next read
        etimer_reset(&et);
    }

    PROCESS_END();
}