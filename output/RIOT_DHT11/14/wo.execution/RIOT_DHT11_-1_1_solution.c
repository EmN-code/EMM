#include <stdio.h>
#include "thread_arch.h"
#include "thread.h"
#include "dht.h"
#include "xtimer.h"
#include "esp_idf_gpio.h"

#define DHT_PIN 2
#define SLEEP_INTERVAL (5U * US_PER_SEC) // Sleep interval in microseconds

static dht_t dev;
static dht_params_t params = {
    .pin = DHT_PIN,
    .type = DHT11
};

void *dht_thread(void *arg) {
    (void)arg;
    int16_t temp, hum;

    if (dht_init(&dev, &params) != DHT_OK) {
        printf("Failed to initialize DHT11\n");
        return NULL;
    }
    printf("DHT11 Initialized\n");

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT11\n");
        }
        xtimer_sleep(SLEEP_INTERVAL / US_PER_SEC);
    }

    return NULL;
}

int main(void) {
    printf("Starting DHT11 example\n");
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, dht_thread, NULL, "dht_thread");
    return 0;
}