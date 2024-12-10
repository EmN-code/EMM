#include <stdio.h>
#include <thread_arch.h>
#include <thread.h>
#include <xtimer.h>
#include <periph/gpio.h>

#include "dht.h"
#include "dht_params.h"

#define DHT_TYPE DHT11
#define DHT_PIN GPIO_NUM_2
#define SLEEP_INTERVAL (5U * 1000000U) // 5 seconds

static dht_t dht_dev;

void *read_dht(void *arg) {
    int16_t temp, hum;
    while (1) {
        if (dht_read(&dht_dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                    temp / 10, temp % 10, 
                    hum / 10, hum % 10);
        } else {
            printf("Failed to read data from DHT11 sensor.\n");
        }
        xtimer_usleep(SLEEP_INTERVAL);
    }
    return NULL;
}

int main(void) {
    printf("Initializing DHT11 sensor on pin %d\n", DHT_PIN);
    dht_params_t params = {
        .pin = DHT_PIN,
        .type = DHT_TYPE
    };

    if (dht_init(&dht_dev, &params) != DHT_OK) {
        printf("Failed to initialize DHT11 sensor\n");
        return 1;
    }

    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, read_dht, NULL, "dht_reader");

    return 0;
}