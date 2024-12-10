#include <thread.h> // RIOT threading is used for delay
#include <xtimer.h> // To use xtimer for time-related functions
#include <stdio.h> // For standard input output
#include <periph/gpio.h> // For LED control
#include "dht.h" // DHT sensor driver

#define DHT11_PIN GPIO_PIN(0, 8) // Assume DHT11 is connected to pin 8
#define DHT_TYPE DHT11 // DHT11 sensor type
#define LED_PIN GPIO_PIN(0, 2) // Assume LED is connected to pin 2

#define TEMP_THRESHOLD 30 // Temperature threshold
#define DELAY 5 // Delay in seconds

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    // Initialize the GPIO pin for the LED
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error: Failed to initialize LED GPIO\n");
        return 1;
    }

    // Initialize the DHT sensor
    if (dht_init(&dev, &DHT11_PIN, DHT_TYPE) != DHT_OK) {
        printf("Error: Failed to initialize DHT sensor\n");
        return 1;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Error: Failed to read from DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);

            // Check if temperature exceeds threshold
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        
        // Wait for the specified delay
        xtimer_sleep(DELAY);
    }

    return 0;
}