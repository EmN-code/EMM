
#include "contiki.h"
#include "dev/leds.h"
#include "dev/dht11.h" // Ensure this header is available
#include <stdio.h>

PROCESS(dht11_led_process, "DHT11 and LED Process");
AUTOSTART_PROCESSES(&dht11_led_process);

#define RGB_LED_WHITE   (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)

PROCESS_THREAD(dht11_led_process, ev, data)
{
    static struct etimer timer;
    static int humidity, temperature;

    PROCESS_BEGIN();

    // Initialize the DHT11 sensor
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, 0); // Replace 0 with the actual GPIO port
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, 0);  // Replace 0 with the actual GPIO pin

    // Set the timer to trigger every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor
        if(dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER, &humidity) == DHT11_STATUS_OKAY &&
           dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER, &temperature) == DHT11_STATUS_OKAY) {
            printf("Humidity: %d %%, Temperature: %d C\n", humidity, temperature);

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

int main() {
    // Start the Contiki system
    contiki_init();

    // Start the process
    process_start(&dht11_led_process, NULL);

    // Enter the Contiki main loop
    while(1) {
        process_run();
    }

    return 0;
}
