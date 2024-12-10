#include "mma8x5x_params.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "periph/gpio.h"
#include "periph/xtimer.h"
#include <stdio.h>

#define LED_PIN GPIO_PIN(PORT_A, 5)  // Replace PORT_A and pin number with appropriate values
#define TEMP_THRESHOLD 30 // Temperature threshold for LED warning.

static dht_t dht_dev;

void *monitor_temperature(void *arg) {
    (void)arg;
    int16_t temperature, humidity;

    // Initialize the DHT sensor
    if (dht_init(&dht_dev, &dht_params[0]) != DHT_OK) {
        printf("Error initializing DHT sensor\n");
        return NULL;
    }

    while (1) {
        // Read values from the DHT sensor
        if (dht_read(&dht_dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                   temperature / 10, temperature % 10, 
                   humidity / 10, humidity % 10);

            // Check if temperature exceeds the threshold
            if (temperature / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Warning: Temperature exceeded threshold!\n");
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read sensor\n");
        }

        // Sleep for 5 seconds
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    // Initialize the GPIO for the LED
    if (gpio_init(LED_PIN, GPIO_OUT) < 0) {
        printf("Error initializing GPIO\n");
        return 1;
    }

    // Creating a thread to handle temperature monitoring
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN -1, 0, 
                  monitor_temperature, NULL, "temp_monitor");

    return 0;
}