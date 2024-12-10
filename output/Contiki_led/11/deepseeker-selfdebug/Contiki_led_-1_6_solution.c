
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "contiki.h"
#include "dev/dht11-sensor.h" // Ensure this header is available
#include "dev/leds.h"

#define RGB_LED_WHITE   (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)

PROCESS(dht11_rgb_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_process);

PROCESS_THREAD(dht11_rgb_process, ev, data)
{
    static struct etimer timer;
    static uint8_t humidity_integer, temperature_integer;

    PROCESS_BEGIN();

    // Initialize the DHT11 sensor
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, 0);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, 1);

    // Set the timer to trigger every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor
        if (dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER, &humidity_integer) == DHT11_STATUS_OKAY &&
            dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER, &temperature_integer) == DHT11_STATUS_OKAY) {
            printf("Humidity: %d %%, Temperature: %d C\n", humidity_integer, temperature_integer);

            // Turn on the RGB LED
            leds_set(RGB_LED_WHITE);
        } else {
            printf("Failed to read DHT11 sensor\n");
        }

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}
