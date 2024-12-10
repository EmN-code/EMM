
#include <stdio.h>
#include "thread.h"  // Include the missing header file
#include "xtimer.h"  // Include xtimer for timing operations
#include "dht11.h"   // Include DHT11 sensor driver
#include "periph/gpio.h"  // Include GPIO for controlling the RGB LED

#define DHT11_PIN GPIO_PIN(0, 15)  // Define the GPIO pin for DHT11
#define RGB_LED_PIN GPIO_PIN(0, 16)  // Define the GPIO pin for RGB LED

// Define the RGB LED colors
#define RGB_LED_WHITE   (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_MAGENTA (RGB_LED_RED | RGB_LED_BLUE)
#define RGB_LED_CYAN    (RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_YELLOW  (RGB_LED_RED | RGB_LED_GREEN)

// Function to read DHT11 sensor data
void read_dht11(void) {
    int16_t temp, hum;
    dht11_read(DHT11_PIN, &temp, &hum);
    printf("Temperature: %d C, Humidity: %d %%\n", temp, hum);
}

// Function to control the RGB LED
void control_rgb_led(int color) {
    gpio_write(RGB_LED_PIN, color);
}

int main(void) {
    // Initialize the DHT11 sensor
    dht11_init(DHT11_PIN);

    // Initialize the RGB LED
    gpio_init(RGB_LED_PIN, GPIO_OUT);

    while (1) {
        // Read DHT11 sensor data
        read_dht11();

        // Turn on the RGB LED
        control_rgb_led(RGB_LED_WHITE);

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}
