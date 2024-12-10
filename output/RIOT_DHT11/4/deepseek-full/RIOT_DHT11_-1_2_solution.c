
#include "periph/i2c.h"
#include "thread.h"
#include "periph/rtc.h"
#include "event/thread.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN             GPIO_PIN(0, 2)
#define DHT_TYPE            DHT11
#define SLEEP_DURATION      (5 * US_PER_SEC)

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
    if (dht_init(&dht_dev, DHT_TYPE, DHT_PIN) == DHT_OK) {
        while (1) {
            read_dht();
            xtimer_sleep(SLEEP_DURATION);
        }
    } else {
        puts("Initialization of DHT sensor failed");
    }

    return 0;
}
