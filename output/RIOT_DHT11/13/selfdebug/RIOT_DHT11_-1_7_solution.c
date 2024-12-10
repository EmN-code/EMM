#include "xtimer.h"
#include "timex.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN     GPIO_PIN(0, 2) // GPIO pin for DHT11
#define DHT_TYPE    DHT11
#define INTERVAL    (5U * US_PER_SEC) // 5 seconds interval

int main(void)
{
    // Initialize DHT sensor
    dht_t dev;
    if (dht_init(&dev, DHT_PIN, DHT_TYPE) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    while (1) {
        int16_t temperature, humidity;
        if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
            printf("Failed to read DHT sensor\n");
        }
        else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        }

        xtimer_usleep(INTERVAL);
    }

    return 0;
}