#include <stdio.h>
#include <stdlib.h>
#include "thread.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_B, 5) // Define correct GPIO pin for DHT sensor
#define LED_PIN GPIO_PIN(PORT_A, 7) // Define correct GPIO pin for LED
#define TEMP_THRESHOLD 25 // Temperature threshold for LED activation

static dht_t dev;

void *temperature_monitor_thread(void *arg) {
    (void)arg;
    int16_t temp, hum;

    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_PIN, DHT11) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return NULL;
    }

    while (1) {
        // Read DHT sensor
        if (dht_read(&dev, &temp, &hum) != 0) {
            printf("Failed to read DHT sensor\n");
        } else {
            // Convert temperature to degrees Celsius
            temp /= 10;
            printf("Temperature: %d C\n", temp);

            // Turn on LED if temperature exceeds threshold
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a thread for temperature monitoring
    char temperature_stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(temperature_stack, sizeof(temperature_stack),
                  THREAD_PRIORITY_MAIN - 1, 0, temperature_monitor_thread,
                  NULL, "temperature_monitor");

    // Main thread just sleeps
    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}