#include <stdio.h>
#include "xtimer.h"
#include "thread_flags.h"
#include "dht.h"
#include "thread_arch.h"
#include "thread.h"
#include "event/timeout.h"
#include "event/periodic.h"

#define DHT_PIN GPIO2
#define DHT_TYPE DHT11
#define MEASURE_INTERVAL (5U * US_PER_SEC)

static dht_t dht_dev;

static void *dht_thread(void *arg) {
    (void)arg;
    int16_t temperature, humidity;
    while (1) {
        if (dht_read(&dht_dev, &temperature, &humidity) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        } else {
            printf("Failed to read data from DHT11 sensor\n");
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    dht_params_t params = {
        .pin = DHT_PIN,
        .type = DHT_TYPE
    };

    if (dht_init(&dht_dev, &params) == 0) {
        printf("DHT11 sensor initialized\n");
        thread_create(NULL, 1024, THREAD_PRIORITY_MAIN - 1,
                      THREAD_CREATE_STACKTEST, dht_thread, NULL, "dht_thread");
    } else {
        printf("Failed to initialize DHT11 sensor\n");
    }

    return 0;
}