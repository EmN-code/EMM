#pragma once
#include "thread_arch.h"
#include "thread_config.h"
#include "timer.h"
#include "dht.h"
#include "xtimer.h"

#define DHT_PIN 2
#define SLEEP_DURATION (5 * 1000000U)

static dht_t dht_dev;

void *dht_read_thread(void *arg) {
    (void)arg;
    int16_t temp, hum;

    while (1) {
        if (dht_read(&dht_dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d\n", temp / 10, temp % 10);
            printf("Humidity: %d.%d\n", hum / 10, hum % 10);
        } else {
            printf("Error reading DHT11\n");
        }
        xtimer_sleep(SLEEP_DURATION / 1000000U);
    }

    return NULL;
}

int main(void) {
    dht_params_t dht_params;
    dht_params.pin = DHT_PIN;
    dht_params.type = DHT11;

    if (dht_init(&dht_dev, &dht_params) != DHT_OK) {
        printf("DHT initialization failed\n");
        return 1;
    }

    thread_create(NULL, THREAD_STACKSIZE_MAIN, THREAD_PRIORITY_MAIN, 0, dht_read_thread, NULL, "dht_read_thread");

    return 0;
}