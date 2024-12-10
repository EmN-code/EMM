
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "periph/rtc.h"
#include "dht.h"

#define DHT11_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static void read_dht11(void)
{
    dht_params_t dht11_params = {
        .pin = DHT11_PIN,
        .type = DHT11,
        .in_mode = DHT_PARAM_PULL,
    };

    dht_t dht11;
    int16_t temp, hum;

    if (dht_init(&dht11, &dht11_params) != DHT_OK) {
        puts("DHT11 initialization failed");
        return;
    }

    while (1) {
        if (dht_read(&dht11, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            puts("Failed to read from DHT11 sensor");
        }

        xtimer_usleep(SLEEP_INTERVAL);
    }
}

int main(void)
{
    read_dht11();
    return 0;
}
