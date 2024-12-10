#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(PORT_A, 2)
#define INTERVAL 5 // 5 seconds

int main(void) {
    dht_t dev;

    if (dht_init(&dev, DHT_TYPE_DHT11, DHT_PIN) != DHT_OK) {
        printf("Failed to initialize DHT11 sensor\n");
        return 1;
    }

    while (1) {
        int16_t temp, hum;

        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Error reading from DHT11 sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        }

        xtimer_sleep(INTERVAL);
    }

    return 0;
}