#include <thread_arch.h>
#include <stdio.h>
#include <stdlib.h>
#include <xtimer.h>
#include <dht.h>
#include <dht_params.h>
#include <periph/gpio.h>

#define DHT_PIN GPIO_PIN(PORT_A, 2)
#define INTERVAL (5U * US_PER_SEC)

static int read_dht(void) {
    int16_t temp, hum;
    dht_t dev;

    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT11;
  
    if (dht_init(&dev, &params) != 0) {
        printf("Failed to initialize DHT11\n");
        return -1;
    }

    if (dht_read(&dev, &temp, &hum) != 0) {
        printf("Failed to read from DHT11\n");
        return -1;
    }

    printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, abs(temp % 10), hum / 10, abs(hum % 10));

    return 0;
}

int main(void) {
    while (1) {
        read_dht();
        xtimer_usleep(INTERVAL);
    }
    return 0;
}