#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(0, 2)
#define INTERVAL (5U * US_PER_SEC) // 5 seconds interval

dht_t dev;

static void read_dht(void) {
    int16_t temp, hum;
    if (dht_read(&dev, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d.%d°C\n", temp / 10, abs(temp % 10));
        printf("Humidity: %d.%d%%\n", hum / 10, hum % 10);
    }
    else {
        printf("Error reading DHT sensor\n");
    }
}

int main(void) {
    if (dht_init(&dev, DHT_TYPE_DHT11, DHT_PIN) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    while (1) {
        read_dht();
        xtimer_sleep(INTERVAL / US_PER_SEC);
    }

    return 0;
}