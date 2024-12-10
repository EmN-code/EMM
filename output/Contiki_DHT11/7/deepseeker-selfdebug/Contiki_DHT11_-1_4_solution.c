
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

    // Configure GPIO pin for DHT11
    gpio_hal_pin_configure(DHT11_PIN, GPIO_HAL_PIN_MODE_INPUT);

    while (1) {
        // Set timer for 5 seconds
        etimer_set(&et, CLOCK_SECOND * 5);

        // Wait for the timer to expire
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Read DHT11 sensor
        if (dht11_read(DHT11_PIN, &temperature, &humidity) == DHT11_SUCCESS) {
            printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
        } else {
            printf("Failed to read DHT11 sensor\n");
        }
    }

    PROCESS_END();
}

int main(void)
{
    // Initialize Contiki
    contiki_init();

    // Start the Contiki system
    contiki_start();

    return 0;
}
