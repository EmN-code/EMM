#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xtimer.h>
#include <thread.h>
#include <dht.h>
#include <periph/gpio.h>

#define DHT11_PIN GPIO_PIN(0, 2) // Assuming gpio0 for board, change accordingly for ESP32

void *thread_func(void *arg) {
    (void)arg;
    dht_t dev;
    int16_t temperature, humidity;

    // Initialize the DHT11 on the specified pin
    if (dht_init(&dev, DHT11_PIN) != DHT_OK) {
        printf("Failed to initialize DHT11 sensor\n");
        return NULL;
    }

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10, humidity / 10, humidity % 10);
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }

        // Sleep for 5 seconds
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    // Create a new thread for reading DHT11
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, thread_func, NULL, "dht11_thread");

    // The main loop, it can perform other tasks if necessary
    while (1) {
        // Just idle here
        xtimer_sleep(3600);
    }

    return 0;
}