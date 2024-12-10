
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "dev/dht11.h"
#include <stdio.h>

#define DHT11_PIN 2

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
    static struct etimer et;
    static int temperature, humidity;

    PROCESS_BEGIN();

    // Set the DHT11 pin as output
    gpio_hal_arch_pin_set_output(DHT11_PIN);

    while(1) {
        // Set the etimer to expire after 5 seconds
        etimer_set(&et, CLOCK_SECOND * 5);

        // Wait for the etimer to expire
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Read the DHT11 sensor
        if(dht11_read(DHT11_PIN, &temperature, &humidity) == 0) {
            printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
        } else {
            printf("Failed to read DHT11 sensor\n");
        }

        // Reset the etimer for the next reading
        etimer_reset(&et);
    }

    PROCESS_END();
}

int main(void)
{
    // Initialize the Contiki system
    contiki_init();

    // Start the Contiki processes
    contiki_process_run();

    return 0;
}
