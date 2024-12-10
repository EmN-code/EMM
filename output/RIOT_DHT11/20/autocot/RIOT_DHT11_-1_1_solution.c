#include <stdio.h>
#include <stdint.h>
#include "thread_arch.h"
#include "event/periodic.h"
#include "dht_params.h"
#include "dht.h"
#include "xtimer.h"

#define DHT_PIN GPIO_NUM_2
#define READ_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static dht_t dht_dev;
static dht_params_t dht_params = {
    .pin = DHT_PIN,
    .type = DHT11
};

void *dht_read_loop(void *arg) {
    int16_t temp, hum;
    while (1) {
        if (dht_read(&dht_dev, &temp, &hum) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read DHT11 sensor\n");
        }
        xtimer_usleep(READ_INTERVAL);
    }
    return NULL;
}

int main(void) {
    printf("Initializing DHT11 sensor\n");
    if (dht_init(&dht_dev, &dht_params) != 0) {
        printf("DHT11 initialization failed\n");
        return 1;
    }
    thread_create(NULL, sizeof(thread_stack), THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST, dht_read_loop, NULL, "dht_reader");
    return 0;
}