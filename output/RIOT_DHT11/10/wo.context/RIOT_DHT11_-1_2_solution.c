#include "xtimer.h"
#include "thread.h"
#include "dht.h"
#include <stdio.h>

#define DHT11_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5U * US_PER_SEC) // 5 seconds in microseconds
#define THREAD_STACKSIZE THREAD_STACKSIZE_MAIN

static char stack[THREAD_STACKSIZE];

void *dht11_thread(void *arg) {
    (void)arg;
    dht_t dev;
    int16_t temperature, humidity;

    // Initialize the DHT11 sensor
    if (dht_init(&dev, DHT11_PIN, DHT11) != DHT_OK) {
        puts("Failed to initialize DHT11 sensor");
        return NULL;
    }

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            printf("DHT11: Temperature = %d.%d°C, Humidity = %d.%d%%\n",
                   temperature / 10, temperature % 10, humidity / 10, humidity % 10);
        } else {
            puts("Failed to read from DHT11 sensor");
        }

        xtimer_usleep(SLEEP_INTERVAL); // Sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN,
                  THREAD_CREATE_STACKTEST, dht11_thread, NULL, "dht11_thread");

    sched_yield(); // Yield to allow the new thread to run
    return 0;
}