#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2) 
#define INTERVAL (5U * US_PER_SEC)

int main(void) {
    dht_params_t params = {
        .pin = DHT_PIN,
        .type = DHT11
    };

    if (dht_init(&params) != DHT_OK) {
        puts("DHT initialization failed");
        return 1;
    }

    while (1) {
        int16_t temp, hum;
        if (dht_read(&params, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            puts("Error reading from DHT sensor");
        }

        xtimer_sleep(INTERVAL);
    }
    return 0;
}