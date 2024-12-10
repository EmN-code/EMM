
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "dev/dht11.h" // Include the correct header file for DHT11

// Define the RGB LED pins
#define RGB_LED_RED    GPIO_PIN(0, 13)
#define RGB_LED_GREEN  GPIO_PIN(0, 14)
#define RGB_LED_BLUE   GPIO_PIN(0, 15)

// Define the DHT11 GPIO pin
#define DHT11_PIN      GPIO_PIN(0, 2)

// Function to read DHT11 sensor data
void read_dht11(void) {
    int8_t temperature, humidity;
    dht11_read(&temperature, &humidity, DHT11_PIN);
    printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
}

// Function to turn on the RGB LED
void turn_on_rgb_led(void) {
    gpio_set(RGB_LED_RED);
    gpio_set(RGB_LED_GREEN);
    gpio_set(RGB_LED_BLUE);
}

// Main function
int main(void) {
    // Initialize the GPIO pins
    gpio_init(RGB_LED_RED, GPIO_OUT);
    gpio_init(RGB_LED_GREEN, GPIO_OUT);
    gpio_init(RGB_LED_BLUE, GPIO_OUT);
    gpio_init(DHT11_PIN, GPIO_IN);

    while (1) {
        // Read DHT11 sensor data
        read_dht11();

        // Turn on the RGB LED
        turn_on_rgb_led();

        // Wait for 5 seconds
        sleep(5);
    }

    return 0;
}
