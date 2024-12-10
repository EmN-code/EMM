
#include <stdio.h>
#include "contiki.h"
#include "dev/dht11-sensor.h" // Include the correct header file
#include "dev/leds.h"

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 1

PROCESS(dht11_led_process, "DHT11 and LED Process");
AUTOSTART_PROCESSES(&dht11_led_process);

PROCESS_THREAD(dht11_led_process, ev, data)
{
    static struct etimer timer;
    static uint8_t temperature, humidity;

    PROCESS_BEGIN();

    // Initialize the DHT11 sensor
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PORT);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);

    etimer_set(&timer, CLOCK_SECOND * 5); // Set timer to 5 seconds

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor
        if(dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER, &humidity) == DHT11_STATUS_OKAY &&
           dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER, &temperature) == DHT11_STATUS_OKAY) {
            printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);

            // Turn on the RGB LED
            leds_set(LEDS_RED);
        } else {
            printf("Failed to read DHT11 sensor\n");
        }

        etimer_reset(&timer); // Reset the timer
    }

    PROCESS_END();
}
