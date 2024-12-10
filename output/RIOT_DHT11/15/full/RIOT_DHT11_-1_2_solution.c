#include "thread.h"
#include "event/thread.h"
#include "dht.h"
#include "xtimer.h"
#include "periph/gpio.h"
#include <stdio.h>

#define DHT_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5U * US_PER_SEC)

static char stack[THREAD_STACKSIZE_MAIN];

void *read_dht11(void *arg) {
    (void)arg;
    dht_t dev;
    int16_t temperature, humidity;

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) != 0) {
            printf("Failed to read DHT11\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                    temperature / 10, temperature % 10, 
                    humidity / 10, humidity % 10);
        }
        xtimer_sleep(SLEEP_INTERVAL);
    }
    return NULL;
}

int main(void) {
    printf("Starting DHT11 reader\n");

    /* Initialize the GPIO pin for the DHT11 sensor */
    if (gpio_init(DHT_PIN, GPIO_IN) != 0) {
        printf("Error: Failed to initialize GPIO pin\n");
        return 1;
    }

    /* Create a thread to read DHT11 data every 5 seconds. */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, read_dht11, NULL, "dht11_reader");

    return 0;
}