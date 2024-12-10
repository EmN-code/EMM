#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN         GPIO_PIN(0, 2) // GPIO 2
#define DHT_INTERVAL    (5U * 1000000U) // 5 seconds in microseconds

int main(void)
{
    dht_t dev;
    int16_t temperature, humidity;
    
    // Initialize the DHT sensor
    if (dht_init(&dev, DHT11, DHT_PIN) != 0) {
        printf("Failed to initialize DHT11\n");
        return 1;
    }

    while (1) {
        // Read values from DHT sensor
        if (dht_read(&dev, &temperature, &humidity) != 0) {
            printf("Failed to read from DHT11\n");
        }
        else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        }

        // Wait for 5 seconds
        xtimer_sleep(DHT_INTERVAL / 1000000U);
    }

    return 0;
}