
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

static dht_t dev;

static void read_dht(void)
{
    int16_t temp, hum;
    int status = dht_read(&dev, &temp, &hum);

    if (status == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
               temp / 10, temp % 10, hum / 10, hum % 10);
    } else {
        printf("Error reading DHT sensor: %d\n", status);
    }
}

int main(void)
{
    if (dht_init(&dev, DHT_PIN, DHT_TYPE_DHT11) == DHT_OK) {
        printf("DHT11 sensor initialized successfully\n");
    } else {
        printf("Failed to initialize DHT11 sensor\n");
        return 1;
    }

    while (1) {
        read_dht();
        xtimer_sleep(INTERVAL / 1000000); // Convert microseconds to seconds
    }

    return 0;
}
