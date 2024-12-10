
#include "contiki.h"
#include "dev/dht11.h"
#include "dev/leds.h"
#include <stdio.h>

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 1

PROCESS(dht11_led_process, "DHT11 and LED Process");
AUTOSTART_PROCESSES(&dht11_led_process);

PROCESS_THREAD(dht11_led_process, ev, data)
{
    static struct etimer timer;
    static int temperature;
    static int humidity;

    PROCESS_BEGIN();

    // Initialize DHT11 sensor
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PORT);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);

    // Set timer to trigger every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read temperature and humidity from DHT11
        if (dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER, &temperature) == DHT11_STATUS_OKAY &&
            dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER, &humidity) == DHT11_STATUS_OKAY) {
            printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);

            // Turn on the RGB LED
            leds_on(LEDS_RED);
            leds_on(LEDS_GREEN);
            leds_on(LEDS_BLUE);
        } else {
            printf("Failed to read DHT11 sensor\n");
        }

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void)
{
    // Start the Contiki system
    contiki_init();

    // Start the process
    process_start(&dht11_led_process, NULL);

    // Enter the Contiki main loop
    while (1) {
        process_run();
    }

    return 0;
}
