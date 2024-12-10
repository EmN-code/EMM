#include <xtimer.h>  // Import RIOT OS xtimer for delays
#include <stdio.h>   // Standard IO for printing to console
#include <periph/gpio.h> // Peripheral GPIO for LED control
#include <periph/i2c.h> // I2C peripheral for sensor communication
#include <dht.h>    // DHT sensor APIs

#define DHT11_PIN GPIO_PIN(PORT_A, 7)  // Define the GPIO pin connected to DHT11
#define LED_PIN GPIO_PIN(PORT_B, 5)    // Define the GPIO pin connected to LED
#define TEMP_THRESHOLD 30              // Temperature threshold for LED warning

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;
    int rc; // Return code for sensor and network functions

    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN); // Turn off the LED initially

    // Proper initialization of DHT sensor
    if (dht_init(&dev, DHT11_PIN, DHT11) != DHT_OK) {
        printf("Error initializing DHT sensor\n");
        return -1;
    }

    while (1) {
        rc = dht_measure(&dev, &temperature, &humidity);
        if (rc != DHT_OK) {
            printf("Failed to measure: %d\n", rc);
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10, humidity / 10, humidity % 10);

            if (temperature / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN); // Turn on LED
            } else {
                gpio_clear(LED_PIN); // Turn off LED
            }
        }

        xtimer_sleep(5);  // Wait for 5 seconds
    }

    return 0; // Typically unreachable
}