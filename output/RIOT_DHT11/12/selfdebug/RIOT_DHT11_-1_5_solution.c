#include <xtimer.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN         GPIO_PIN(0, 2)
#define DELAY           (5U * US_PER_SEC) // 5 seconds delay

int main(void)
{
    dht_params_t dht_params;
    dht_params.type = DHT11;
    dht_params.pin = DHT_PIN;

    dht_t dev;
    if (dht_init(&dev, &dht_params) != DHT_OK) {
        printf("Failed to initialize DHT11 sensor\n");
        return 1;
    }

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read data from DHT11 sensor\n");
        }
        xtimer_sleep(5);
    }
    return 0;
}