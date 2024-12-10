#include <xtimer.h>
#include <stdio.h>
#include "dht.h"
#include "dht_params.h"
#include "timex.h"

#define INTERVAL_USEC (5U * US_PER_SEC) // 5 seconds interval
#define DHT11_PIN GPIO_PIN(0, 2)      // GPIO2 for DHT11

int main(void)
{
    dht_t dev;
    int16_t temp, hum;

    // Initialize the DHT11 sensor
    if (dht_init(&dev, &dht_params[0]) != DHT_OK) {
        printf("DHT init failed\n");
        return 1;
    }

    printf("DHT11 sensor initialized\n");

    while (1) {
        // Read values from DHT11
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10,
                   hum / 10, hum % 10);
        } else {
            printf("Failed to read DHT11 sensor\n");
        }

        // Sleep for the defined interval
        xtimer_usleep(INTERVAL_USEC);
    }

    return 0;
}