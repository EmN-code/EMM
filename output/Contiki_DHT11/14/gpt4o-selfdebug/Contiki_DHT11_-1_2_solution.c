// Description: This program is designed for an NRF52840 platform using Contiki OS to read data from a DHT11 sensor every 5 seconds. The DHT11 sensor is connected to GPIO 2. This version corrects a previous error where a mistyped sensor name was referenced.

// Import necessary headers in descending alphabetical order
#include "sys/timer.h"
#include "sys/etimer.h"
#include "sys/ctimer.h"
#include "contiki.h"

PROCESS(dht11_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_process);

static struct etimer timer;

PROCESS_THREAD(dht11_process, ev, data) {
    PROCESS_BEGIN();

    // Initialize DHT11 reader
    static dht11_reader dht11;

    // Set GPIO pin for the DHT11
    dht11_init(&dht11, 2);

    while (1) {
        // Set the timer for 5 seconds
        etimer_set(&timer, CLOCK_SECOND * 5);

        // Wait for the timer event
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);

        // Read data from the DHT11 sensor
        int temperature, humidity;
        dht11_read(&dht11, &temperature, &humidity);

        // Print the data
        printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
    }

    PROCESS_END();
}