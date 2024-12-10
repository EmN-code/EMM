
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "contiki.h"
#include "dev/dht11-sensor.h"
#include "dev/leds.h"

// Define the RGB LED colors
#define RGB_LED_RED   0x01
#define RGB_LED_GREEN 0x02
#define RGB_LED_BLUE  0x04

// Define the RGB LED color to turn on
#define RGB_LED_COLOR (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)

// Function to read DHT11 sensor data
static void read_dht11(void) {
    uint8_t humidity_int, humidity_dec, temperature_int, temperature_dec;
    int status = dht11_sensor.value(&dht11_sensor, DHT11_VALUE_HUMIDITY_INTEGER, &humidity_int);
    if (status == DHT11_STATUS_OKAY) {
        dht11_sensor.value(&dht11_sensor, DHT11_VALUE_HUMIDITY_DECIMAL, &humidity_dec);
        dht11_sensor.value(&dht11_sensor, DHT11_VALUE_TEMPERATURE_INTEGER, &temperature_int);
        dht11_sensor.value(&dht11_sensor, DHT11_VALUE_TEMPERATURE_DECIMAL, &temperature_dec);
        printf("DHT11 - Humidity: %d.%d%%, Temperature: %d.%dC\n", humidity_int, humidity_dec, temperature_int, temperature_dec);
    } else {
        printf("Failed to read DHT11 sensor\n");
    }
}

// Main function
int main(void) {
    // Initialize the DHT11 sensor
    dht11_sensor.configure(&dht11_sensor, DHT11_CONFIGURE_GPIO_PORT, 0);
    dht11_sensor.configure(&dht11_sensor, DHT11_CONFIGURE_GPIO_PIN, 1);

    // Initialize the RGB LED
    leds_init();

    while (1) {
        // Read the DHT11 sensor
        read_dht11();

        // Turn on the RGB LED
        leds_set(RGB_LED_COLOR);

        // Wait for 5 seconds
        clock_delay(5 * CLOCK_SECOND);

        // Turn off the RGB LED
        leds_off(RGB_LED_COLOR);

        // Wait for a short delay before the next reading
        clock_delay(CLOCK_SECOND);
    }

    return 0;
}
