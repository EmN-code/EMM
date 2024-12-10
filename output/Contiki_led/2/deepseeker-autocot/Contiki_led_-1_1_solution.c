
#include "contiki.h"
#include "dev/leds.h"
#include "dev/dht11.h"
#include <stdio.h>

// Define the RGB LED pins
#define RED_LED_PIN 22
#define GREEN_LED_PIN 23
#define BLUE_LED_PIN 24

// Function to initialize the RGB LED pins
void init_rgb_led() {
    // Initialize the pins as outputs
    nrf_gpio_cfg_output(RED_LED_PIN);
    nrf_gpio_cfg_output(GREEN_LED_PIN);
    nrf_gpio_cfg_output(BLUE_LED_PIN);

    // Turn off the LEDs initially
    nrf_gpio_pin_clear(RED_LED_PIN);
    nrf_gpio_pin_clear(GREEN_LED_PIN);
    nrf_gpio_pin_clear(BLUE_LED_PIN);
}

// Function to turn on the RGB LED
void turn_on_rgb_led() {
    nrf_gpio_pin_set(RED_LED_PIN);
    nrf_gpio_pin_set(GREEN_LED_PIN);
    nrf_gpio_pin_set(BLUE_LED_PIN);
}

// Function to turn off the RGB LED
void turn_off_rgb_led() {
    nrf_gpio_pin_clear(RED_LED_PIN);
    nrf_gpio_pin_clear(GREEN_LED_PIN);
    nrf_gpio_pin_clear(BLUE_LED_PIN);
}

// Process to read DHT11 sensor and control the RGB LED
PROCESS(dht11_rgb_led_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_led_process);

PROCESS_THREAD(dht11_rgb_led_process, ev, data) {
    static struct etimer timer;
    static int temperature, humidity;

    PROCESS_BEGIN();

    // Initialize the RGB LED
    init_rgb_led();

    // Set the timer to 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        // Wait for the timer to expire
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor
        if(dht11_read(&temperature, &humidity) == 0) {
            printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);

            // Turn on the RGB LED
            turn_on_rgb_led();

            // Wait for a short period to see the LED on
            etimer_set(&timer, CLOCK_SECOND * 1);
            PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

            // Turn off the RGB LED
            turn_off_rgb_led();
        } else {
            printf("Failed to read DHT11 sensor\n");
        }

        // Reset the timer for the next 5 seconds
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main() {
    // Initialize the Contiki system
    contiki_init();

    // Start the Contiki process
    process_start(&dht11_rgb_led_process, NULL);

    // Enter the Contiki main loop
    while(1) {
        process_run();
    }

    return 0;
}
