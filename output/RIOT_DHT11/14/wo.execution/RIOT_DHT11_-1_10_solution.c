#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "dht_params.h"
#include "periph/gpio.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define DHT_TYPE DHT11

static dht_params_t dht_params = {
    .pin = DHT_PIN,
    .type = DHT_TYPE
};

int main(void) {
    dht_init(&dht_params);
    int16_t temp, hum;

    while (1) {
        if (dht_read(&dht_params, &temp, &hum) != 0) {
            printf("Error reading DHT11\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        }
        xtimer_sleep(5);  // Sleep for 5 seconds
    }
    return 0;
}