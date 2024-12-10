#include <xtimer.h>
#include <timex.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT11_PIN GPIO_PIN(0, 2)  // GPIO 2

int main(void)
{
    dht_t dev;
    int16_t temperature, humidity;

    // Initialize the DHT sensor
    if (dht_init(&dev, DHT11_PIN, DHT11) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    while (1) {
        // Read data from the sensor
        if (dht_read(&dev, &humidity, &temperature) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        } else {
            printf("Failed to read from DHT sensor\n");
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}