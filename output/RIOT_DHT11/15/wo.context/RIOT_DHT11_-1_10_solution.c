#include <xtimer.h>
#include <stdio.h>
#include <dht.h>
#include <periph/gpio.h>

#define DHT_PIN GPIO_PIN(0, 2)  // GPIO 2 for DHT11

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    if (dht_init(&dev, DHT_PIN, DHT11) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    while (1) {
        while (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Error reading the DHT sensor\n");
            xtimer_sleep(1);
        }
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        xtimer_sleep(5);
    }

    return 0;
}