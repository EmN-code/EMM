
#include <stdio.h>
#include "thread.h"
#include "xtimer.h"
#include "periph/gpio.h"
#include "dev_leds.h"

#define DHT11_PIN GPIO_PIN(0, 15)  // Example GPIO pin for DHT11
#define RGB_LED_PIN GPIO_PIN(0, 16) // Example GPIO pin for RGB LED

// Simulated DHT11 reading function
int read_dht11(int pin, int *temperature, int *humidity) {
    // Simulate reading from DHT11
    *temperature = 25;  // Simulated temperature value
    *humidity = 50;     // Simulated humidity value
    return 0;  // Success
}

// Function to toggle the RGB LED
void toggle_rgb_led(void) {
    static int state = 0;
    gpio_write(RGB_LED_PIN, state);
    state = !state;
}

int main(void) {
    int temperature, humidity;

    // Initialize GPIO pins
    gpio_init(DHT11_PIN, GPIO_OUT);
    gpio_init(RGB_LED_PIN, GPIO_OUT);

    while (1) {
        // Read DHT11 sensor
        if (read_dht11(DHT11_PIN, &temperature, &humidity) == 0) {
            printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
            // Toggle RGB LED
            toggle_rgb_led();
        } else {
            puts("Failed to read DHT11 sensor");
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}
