#include <xtimer.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(0, 2)
#define DHT_TYPE DHT11
#define DELAY (5U * US_PER_SEC)

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    if (dht_init(&dev, DHT_TYPE, DHT_PIN) != DHT_OK) {
        puts("Failed to initialize DHT sensor");
        return 1;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            puts("Failed to read from DHT sensor");
        }
        xtimer_sleep(DELAY);
    }
    return 0;
}