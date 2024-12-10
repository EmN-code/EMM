
#include "periph/gpio.h"
#include "thread.h"
#include "event/thread.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define DHT_TYPE DHT11

static dht_t dht_dev;

static void read_dht(void)
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
    if (dht_init(&dht_dev, DHT_TYPE, DHT_PIN) != 0) {
        puts("Initialization of DHT sensor failed");
        return 1;
    }

    while (1) {
        read_dht();
        xtimer_sleep(5);
    }

    return 0;
}
