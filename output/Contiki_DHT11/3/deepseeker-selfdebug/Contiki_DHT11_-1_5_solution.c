
#include "contiki.h"
#include "dev/gpio.h"
#include "dev/dht11.h"
#include <stdio.h>

#define DHT11_PIN 2

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
    static struct etimer timer;
    uint8_t temperature_integer, temperature_decimal;

    PROCESS_BEGIN();

    // Initialize the DHT11 sensor
    dht11_init(DHT11_PIN);

    // Set the timer to trigger every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        // Wait for the timer to expire
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the temperature from the DHT11 sensor
        if(dht11_read(&temperature_integer, &temperature_decimal) == DHT11_OK) {
            printf("Temperature: %d.%d C\n", temperature_integer, temperature_decimal);
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }

        // Reset the timer to trigger again in 5 seconds
        etimer_reset(&timer);
    }

    PROCESS_END();
}
