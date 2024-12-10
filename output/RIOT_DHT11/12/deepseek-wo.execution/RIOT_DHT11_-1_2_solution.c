
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "dht.h"
#include "event/thread.h"
#include "thread.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define DHT_TYPE DHT11

static dht_t dht_dev;

void read_dht(void)
{
    int16_t temp, hum;
    if (dht_read(&dht_dev, &temp, &hum) == 0) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
    } else {
        puts("Failed to read from DHT sensor");
    }
}

int main(void)
{
    if (gpio_init_int(DHT_PIN, GPIO_IN_PU, GPIO_RISING, read_dht, NULL) < 0) {
        puts("Error initializing GPIO");
        return 1;
    }

    dht_params_t dht_params = {
        .pin = DHT_PIN,
        .type = DHT_TYPE,
        .in_mode = GPIO_IN_PU
    };

    if (dht_init(&dht_dev, &dht_params) != 0) {
        puts("Error initializing DHT sensor");
        return 1;
    }

    while (1) {
        read_dht();
        xtimer_sleep(5);
    }

    return 0;
}
