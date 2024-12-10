
#include "contiki.h"
#include "dev/dht11.h"  // Include the correct header file for DHT11 sensor
#include "dev/leds.h"   // Include the header file for LED control
#include <stdio.h>

// Define the RGB LED colors
#define RGB_LED_RED   0x01
#define RGB_LED_GREEN 0x02
#define RGB_LED_BLUE  0x04

// Function to set the RGB LED color
void rgb_led_set(uint8_t color) {
    leds_off(LEDS_ALL);  // Turn off all LEDs
    leds_on(color);      // Turn on the specified color
}

// Process to read DHT11 sensor and control RGB LED
PROCESS(dht11_rgb_led_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_led_process);

PROCESS_THREAD(dht11_rgb_led_process, ev, data) {
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

            // Turn on the RGB LED (e.g., set to red)
            rgb_led_set(RGB_LED_RED);
        } else {
            printf("Failed to read DHT11 sensor\n");
        }

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void) {
    // Initialize the Contiki system
    contiki_init();

    // Start the Contiki processes
    contiki_process_run();

    return 0;
}
