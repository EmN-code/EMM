#include "contiki.h"
#include "contiki-default-conf.h"

#include <stdio.h>

// Timer interval for 5 seconds
#define TIMER_INTERVAL (CLOCK_SECOND * 5)

PROCESS(dht11_reader_process, "DHT11 Reader Process");
AUTOSTART_PROCESSES(&dht11_reader_process);

// Placeholder for DHT11 reading function. Requires the correct library to be used
static int read_dht11_sensor(void) {
    // Simulated value; actual implementation will read from the DHT11
    return 25;
}

PROCESS_THREAD(dht11_reader_process, ev, data)
{
    static struct etimer timer;
    static int temperature;

    PROCESS_BEGIN();

    // Initialize timer to execute every TIMER_INTERVAL
    etimer_set(&timer, TIMER_INTERVAL);

    while(1) {
        // Wait for the timer event
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);

        // Call the function to read DHT11 sensor
        temperature = read_dht11_sensor();
        printf("DHT11 Temperature: %d°C\n", temperature);

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}
