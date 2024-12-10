
#include <stdio.h>
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "dev/dht11.h" // Ensure this header is available or replace with appropriate DHT11 driver

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 2

PROCESS(dht11_reading_process, "DHT11 Reading Process");
AUTOSTART_PROCESSES(&dht11_reading_process);

PROCESS_THREAD(dht11_reading_process, ev, data)
{
    static struct etimer timer;
    static int temperature, humidity;

    PROCESS_BEGIN();

    // Configure GPIO for DHT11
    gpio_hal_pin_configure(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_HAL_PIN_MODE_INPUT);

    // Set timer to read DHT11 every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read DHT11 data
        if(dht11_read(DHT11_GPIO_PORT, DHT11_GPIO_PIN, &temperature, &humidity) == 0) {
            printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
        } else {
            printf("Failed to read DHT11 sensor\n");
        }

        // Reset timer for the next reading
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void)
{
    // Initialize Contiki
    contiki_init();

    // Start the DHT11 reading process
    process_start(&dht11_reading_process, NULL);

    // Enter the Contiki main loop
    while(1) {
        process_run();
    }

    return 0;
}
