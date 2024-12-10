#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <thread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "dht.h"
#include "periph/gpio.h"

#define DHT11_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5U * US_PER_SEC)

static char stack[THREAD_STACKSIZE_DEFAULT];

void *read_dht(void *arg) {
    dht_params_t params;
    params.pin = DHT11_PIN;
    params.type = DHT11;
    
    dht_t dev;
    if (dht_init(&dev, &params) != DHT_OK) {
        printf("DHT initialization failed\n");
        return NULL;
    }

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read data from sensor\n");
        }
        ztimer_sleep(ZTIMER_USEC, SLEEP_INTERVAL);
    }
    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  read_dht, NULL, "dht_reader");
    return 0;
}