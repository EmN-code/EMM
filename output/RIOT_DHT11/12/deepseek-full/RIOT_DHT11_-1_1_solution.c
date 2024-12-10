
#include <stdio.h>
#include "thread.h"
#include "xtimer.h"
#include "dht.h"
#include "periph/gpio.h"

#define DHT_PIN GPIO_PIN(0, 2)  // GPIO 2 on ESP32
#define SLEEP_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static dht_t dht_dev;
static dht_params_t dht_params = {
    .pin = DHT_PIN,
    .type = DHT11,
    .in_mode = DHT_PARAM_PULL,
};

void read_dht11(void) {
    int16_t temp, hum;
    int res;

    res = dht_read(&dht_dev, &temp, &hum);
    if (res == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
               temp / 10, temp % 10, hum / 10, hum % 10);
    } else {
        printf("Error reading DHT11: %d\n", res);
    }
}

int main(void) {
    // Initialize the DHT11 sensor
    if (dht_init(&dht_dev, &dht_params) != DHT_OK) {
        puts("Error initializing DHT11 sensor");
        return 1;
    }

    while (1) {
        read_dht11();
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}
