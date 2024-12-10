
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "contiki-default-conf.h"
#include "dev/leds.h"

// Define the GPIO pins for the RGB LED
#define RED_LED_PIN   22
#define GREEN_LED_PIN 23
#define BLUE_LED_PIN  24

// Define the GPIO pin for the DHT11 sensor
#define DHT11_PIN     25

// Function to set the RGB LED color
void rgb_led_set(uint8_t colour) {
    gpio_hal_set(RED_LED_PIN, (colour & 0x01) ? 1 : 0);
    gpio_hal_set(GREEN_LED_PIN, (colour & 0x02) ? 1 : 0);
    gpio_hal_set(BLUE_LED_PIN, (colour & 0x04) ? 1 : 0);
}

// Function to read the DHT11 sensor
int dht11_read(uint8_t *humidity, uint8_t *temperature) {
    // Simulated DHT11 reading for demonstration purposes
    *humidity = 50;
    *temperature = 25;
    return 0; // Success
}

PROCESS(dht11_rgb_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_process);

PROCESS_THREAD(dht11_rgb_process, ev, data) {
    static struct etimer timer;
    static uint8_t humidity, temperature;

    PROCESS_BEGIN();

    // Initialize GPIO pins for the RGB LED
    gpio_hal_init();
    gpio_hal_output(RED_LED_PIN);
    gpio_hal_output(GREEN_LED_PIN);
    gpio_hal_output(BLUE_LED_PIN);

    // Set the initial color of the RGB LED
    rgb_led_set(0); // Turn off all colors

    while (1) {
        // Set a timer for 5 seconds
        etimer_set(&timer, CLOCK_SECOND * 5);

        // Wait for the timer to expire
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor
        if (dht11_read(&humidity, &temperature) == 0) {
            // Turn on the RGB LED based on the temperature
            if (temperature < 20) {
                rgb_led_set(0x01); // Red
            } else if (temperature < 30) {
                rgb_led_set(0x02); // Green
            } else {
                rgb_led_set(0x04); // Blue
            }
        }

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}
