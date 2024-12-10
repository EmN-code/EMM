#include <xtimer.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5U * US_PER_SEC)

static dht_t dev;

int main(void) {
    int16_t temp, hum;

    dht_init(&dev, DHT11, DHT_PIN);

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from sensor\n");
        }
        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}
