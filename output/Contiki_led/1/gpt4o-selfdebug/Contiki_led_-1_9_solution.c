/* Description: This code reads data from a DHT11 sensor every five seconds and controls an RGB LED on an NRF52840 board using Contiki. */

#include "sensors.h"
#include "dev/leds.h"
#include "dht11_sensor.h"
#include <stdio.h>

// Declare process
PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

// Timer for periodic execution
static struct etimer timer;

// Main process thread
PROCESS_THREAD(dht11_read_process, ev, data) {
    PROCESS_BEGIN();

    while(1) {
        // Set timer for 5 seconds
        etimer_set(&timer, CLOCK_SECOND * 5);

        // Wait for timer event
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read DHT11 sensor values
        int temp_integer = sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
        int temp_decimal = sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL);
        int humidity_integer = sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
        int humidity_decimal = sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL);

        if (temp_integer != -1) { // Check if reading is successful
            printf("Temperature: %d.%d C, Humidity: %d.%d %%\n", temp_integer, temp_decimal, humidity_integer, humidity_decimal);
        }
        else {
            printf("Failed to read from DHT11 sensor\n");
        }

        // Turn on the RGB LED
        leds_on(LEDS_RED);
        leds_on(LEDS_GREEN);
        leds_on(LEDS_BLUE);
    }

    PROCESS_END();
}