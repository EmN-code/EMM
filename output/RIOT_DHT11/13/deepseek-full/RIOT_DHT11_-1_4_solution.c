
#include "thread.h"
#include "dht_params.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "dht.h"

#define DHT11_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5 * 1000 * 1000UL) // 5 seconds in microseconds

static void read_dht11(void) {
    dht_params_t params = {
        .pin = DHT11_PIN,
        .type = DHT11
    };

    dht_t dev;
    int16_t temp, hum;

    if (dht_init(&dev, &params) != DHT_OK) {
        puts("DHT11 initialization failed");
        return;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            puts("Failed to read from DHT11 sensor");
        }

        xtimer_usleep(SLEEP_INTERVAL);
    }
}

int main(void) {
    read_dht11();
    return 0;
}
