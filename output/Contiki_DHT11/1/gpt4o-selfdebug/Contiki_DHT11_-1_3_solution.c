#include "contiki.h"
#include "contiki-net.h"
#include "contiki-lib.h"
#include "contiki-conf.h"
#include <stdio.h>

#define DHT11_PORT GPIO_PIN_PORT2

// Function to simulate reading temperature from DHT11 sensor
int read_temperature() {
    // This is a placeholder function as we cannot interface with hardware directly.
    // In actual implementation, you will perform the GPIO operations to get data from the sensor.
    return 25; // return a mock temperature value
}

PROCESS(read_dht11_process, "DHT11 Reading Process");
AUTOSTART_PROCESSES(&read_dht11_process);

PROCESS_THREAD(read_dht11_process, ev, data)
{
    static struct etimer timer;
    static int temperature;

    PROCESS_BEGIN();

    // Set the timer to fire every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        // Wait for the timer event
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);

        // Read Temperature from DHT11 sensor
        temperature = read_temperature();
        printf("Temperature: %d°C\n", temperature);

        // Reset the timer for another 5 seconds
        etimer_reset(&timer);
    }

    PROCESS_END();
}