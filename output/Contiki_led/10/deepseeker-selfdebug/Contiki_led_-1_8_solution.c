
#include "contiki.h"
#include "dev/dht11-sensor.h"
#include "dev/leds.h"
#include <stdio.h>

// Define the GPIO pins for the RGB LED
#define RED_LED_PIN    (22)
#define GREEN_LED_PIN  (23)
#define BLUE_LED_PIN   (24)

// Define the DHT11 sensor GPIO pin
#define DHT11_GPIO_PIN (25)

// Process to read DHT11 sensor and control RGB LED
PROCESS(dht11_rgb_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_process);

PROCESS_THREAD(dht11_rgb_process, ev, data)
{
    static struct etimer timer;
    static int humidity, temperature;

    PROCESS_BEGIN();

    // Initialize the DHT11 sensor
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);

    // Set the timer to trigger every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor
        if (dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER, &humidity) == DHT11_STATUS_OKAY &&
            dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER, &temperature) == DHT11_STATUS_OKAY) {
            printf("Humidity: %d %%, Temperature: %d C\n", humidity, temperature);

            // Turn on the RGB LED based on temperature
            if (temperature < 20) {
                leds_off(LEDS_RED | LEDS_GREEN | LEDS_BLUE);
                leds_on(LEDS_BLUE);
            } else if (temperature >= 20 && temperature < 30) {
                leds_off(LEDS_RED | LEDS_GREEN | LEDS_BLUE);
                leds_on(LEDS_GREEN);
            } else {
                leds_off(LEDS_RED | LEDS_GREEN | LEDS_BLUE);
                leds_on(LEDS_RED);
            }
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

    // Start the DHT11 and RGB LED process
    process_start(&dht11_rgb_process, NULL);

    return 0;
}
