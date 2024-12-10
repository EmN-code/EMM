
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5 * 1000 * 1000U) /* 5 seconds */

static void read_dht11(void) {
    dht_params_t params = {
        .pin = DHT_PIN,
        .type = DHT11,
        .in_mode = DHT_PARAM_PULL, // Ensure this is defined or replaced with appropriate value
    };

    dht_t dev;
    int16_t temp, hum;

    if (dht_init(&dev, &params) == DHT_OK) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   (int)temp / 10, (int)temp % 10, (int)hum / 10, (int)hum % 10);
        } else {
            puts("Error reading DHT11 sensor");
        }
    } else {
        puts("Error initializing DHT11 sensor");
    }
}

int main(void) {
    while (1) {
        read_dht11();
        xtimer_usleep(SLEEP_INTERVAL);
    }
    return 0;
}
