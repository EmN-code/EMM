
#include "contiki.h"
#include "dev/gpio.h"
#include "dev/dht11.h"
#include <stdio.h>

#define DHT11_PIN 2

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

static struct etimer timer;

PROCESS_THREAD(dht11_read_process, ev, data)
{
    PROCESS_BEGIN();

    // Initialize the DHT11 sensor
    dht11_sensor.configure(DHT11_PIN);

    while(1) {
        // Set the timer to 5 seconds
        etimer_set(&timer, CLOCK_SECOND * 5);

        // Wait for the timer to expire
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor
        int temperature, humidity;
        if(dht11_read(&temperature, &humidity) == 0) {
            printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
        } else {
            printf("Failed to read DHT11 sensor\n");
        }
    }

    PROCESS_END();
}

int main(void)
{
    // Initialize the Contiki system
    contiki_init();

    // Start the DHT11 read process
    process_start(&dht11_read_process, NULL);

    // Enter the Contiki main loop
    while(1) {
        process_run();
    }

    return 0;
}
