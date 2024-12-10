
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN             GPIO_PIN(0, 2)
#define DHT_TYPE            DHT11
#define SLEEP_INTERVAL      (5 * 1000000U)  // 5 seconds in microseconds

static void read_dht(void) {
    dht_params_t params = {
        .pin = DHT_PIN,
        .type = DHT_TYPE,
        .in_mode = GPIO_IN_PU  // Use GPIO_IN_PU instead of DHT_PARAM_PULL
    };

    dht_t dev;
    int16_t temp, hum;

    if (dht_init(&dev, &params) != DHT_OK) {
        puts("DHT11 initialization failed");
        return;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            puts("Failed to read from DHT11 sensor");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        }

        xtimer_usleep(SLEEP_INTERVAL);
    }
}

int main(void) {
    read_dht();
    return 0;
}
