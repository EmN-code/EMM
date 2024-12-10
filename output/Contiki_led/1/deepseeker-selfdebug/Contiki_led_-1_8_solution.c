
#include "contiki.h"
#include "dev/dht11-sensor.h" // Ensure this header is included
#include "dev/leds.h"
#include <stdio.h>

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 1

#define RGB_LED_RED   0x01
#define RGB_LED_GREEN 0x02
#define RGB_LED_BLUE  0x04

PROCESS(dht11_rgb_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_process);

PROCESS_THREAD(dht11_rgb_process, ev, data)
{
    static struct etimer timer;
    static int humidity, temperature;

    PROCESS_BEGIN();

    // Initialize DHT11 sensor
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PORT);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);

    etimer_set(&timer, CLOCK_SECOND * 5); // Set timer to 5 seconds

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read DHT11 sensor values
        humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
        temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

        printf("Humidity: %d %%, Temperature: %d C\n", humidity, temperature);

        // Turn on RGB LED based on temperature
        if (temperature < 20) {
            leds_set(RGB_LED_BLUE);
        } else if (temperature >= 20 && temperature < 30) {
            leds_set(RGB_LED_GREEN);
        } else {
            leds_set(RGB_LED_RED);
        }

        etimer_reset(&timer); // Reset the timer
    }

    PROCESS_END();
}
