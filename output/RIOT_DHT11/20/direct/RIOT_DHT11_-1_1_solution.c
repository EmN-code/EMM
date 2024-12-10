#include <ztimer.h>
#include <dht.h>
#include <stdio.h>
#include <stdint.h>
#include <periph/gpio.h>

#define DHT_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5U * 1000000U) // 5 seconds in microseconds

static dht_t dev;

void read_dht11(void) {
    int16_t temp, hum;
    int res = dht_read(&dev, &temp, &hum);
    if (res == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
    } else {
        printf("Error reading DHT11 sensor: %d\n", res);
    }
}

int main(void) {
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT11;

    if (dht_init(&dev, &params) != DHT_OK) {
        printf("DHT11 initialization error\n");
        return 1;
    }

    while (1) {
        read_dht11();
        ztimer_sleep(ZTIMER_USEC, SLEEP_INTERVAL);
    }

    return 0;
}