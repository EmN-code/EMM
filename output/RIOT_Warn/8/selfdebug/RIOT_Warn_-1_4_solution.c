#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <thread.h>
#include <stdlib.h>
#include <dht.h>
#include <timex.h>
#include <periph/gpio.h>

#define DHT11_PIN GPIO_PIN(PORT_B, 5)  // Define the pin where the DHT11 is connected
#define LED_PIN GPIO_PIN(PORT_A, 2)   // Define the pin where the LED is connected
#define TEMP_THRESHOLD 25             // Temperature threshold for LED warning
#define SLEEP_INTERVAL (5U * US_PER_SEC) // Sleep interval of 5 seconds

static dht_t dev;

int main(void) {
    int16_t temperature, humidity;

    // Initialize the DHT11 sensor
    if (dht_init(&dev, DHT11_PIN) != DHT_OK) {
        printf("Failed to initialize DHT11\n");
        return 1;
    }

    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        // Read data from DHT11
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, abs(temperature % 10),
                   humidity / 10, abs(humidity % 10));

            // Check temperature and toggle LED
            if (temperature / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Warning: Temperature threshold exceeded!\n");
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from DHT11\n");
        }

        // Sleep for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}