#include "xtimer.h"
#include "thread.h"
#include "dht.h"
#include "gpio.h"
#include <stdio.h>

#define LED_PIN GPIO_PIN(0, 15) // Configure the correct pin for your board
#define THRESHOLD_TEMP 30 // Example threshold temperature in Celsius
#define DELAY_SEC 5 // Delay time in seconds

dht_t dev;

int main(void) {
    // Initialize the DHT device with appropriate parameters
    if (dht_init(&dev, 0, DHT11) != DHT_OK) {
        printf("Error initializing DHT sensor\n");
        return 1;
    }

    // Initialize the LED pin
    if (gpio_init(LED_PIN, GPIO_OUT) != 0) {
        printf("Failed to initialize LED pin\n");
        return 1;
    }

    int16_t temperature, humidity;
    while (1) {
        if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
            printf("Failed to read sensor data\n");
        } else {
            printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
            if (temperature > THRESHOLD_TEMP) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }

        // Wait 5 seconds
        xtimer_sleep(DELAY_SEC);
    }
    return 0;
}