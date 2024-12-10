#include "sys/etimer.h"
#include "contiki.h"

#include <stdio.h>

// Initialize the etimer structure
static struct etimer et;

PROCESS(read_dht11_and_print_process, "Read DHT11 and Print Process");
AUTOSTART_PROCESSES(&read_dht11_and_print_process);

PROCESS_THREAD(read_dht11_and_print_process, ev, data)
{
    PROCESS_BEGIN();

    // Set the timer to fire every 5 seconds
    etimer_set(&et, CLOCK_SECOND * 5);

    while(1) {
        // Wait for the timer event
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);

        // Read the sensors (you must implement the dht11 read code or call the function if available)
        // For example purposes, let's assume you have a function dht11_read(int gpio_pin) that reads
        // and returns temperature and humidity
        int temperature, humidity;
        int status = dht11_read(2, &temperature, &humidity);

        if (status == 0) {
            printf("Temperature: %d\n", temperature);
            printf("Humidity: %d\n", humidity);
        } else {
            printf("Failed to read from DHT11\n");
        }

        // Reset the timer to fire again after 5 seconds
        etimer_reset(&et);
    }

    PROCESS_END();
}