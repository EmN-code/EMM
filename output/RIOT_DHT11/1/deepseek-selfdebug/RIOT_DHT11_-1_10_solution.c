
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define INTERVAL (5 * 1000 * 1000U)  // 5 seconds in microseconds

static dht_t dht_dev;

void read_dht(void) {
    int16_t temp, hum;
    if (dht_read(&dht_dev, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
               temp / 10, temp % 10, hum / 10, hum % 10);
    } else {
        puts("Failed to read from DHT sensor");
    }
}

int main(void) {
    // Initialize the DHT11 sensor
    if (dht_init(&dht_dev, DHT_TYPE_DHT11, DHT_PIN) != DHT_OK) {
        puts("Failed to initialize DHT sensor");
        return 1;
    }

    while (1) {
        read_dht();
        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    return 0;
}
