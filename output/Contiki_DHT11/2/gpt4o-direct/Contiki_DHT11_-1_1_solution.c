// This is a simple Contiki OS application that reads a DHT11 sensor every 5 seconds on an nRF52840 board.
// The DHT11 sensor is connected to GPIO pin 2.

#include <stdio.h>
#include "contiki.h"
#include "dev/dht11.h"

PROCESS(dht11_process, "DHT11 Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data)
{
    static struct etimer timer;
    PROCESS_BEGIN();

    // Initialize the timer to trigger every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        // Wait for the timer event
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor
        if (dht11_read() == DHT11_ERROR) {
            printf("Error reading DHT11 sensor\n");
        } else {
            int temperature = dht11_read_temperature();
            int humidity = dht11_read_humidity();
            printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
        }

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}