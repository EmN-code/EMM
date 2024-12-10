#include <ztimer.h>
#include <string.h>
#include <stdio.h>
#include <dht.h>
#include <xtimer.h>
#include <periph/gpio.h>

#define DHT_PIN GPIO_PIN(0, 2)  // DHT11 connected to GPIO2
#define INTERVAL (5U * 1000000U)  // 5 seconds

int main(void)
{
    dht_t dev;
    int16_t temperature, humidity;
    
    // Initialize the DHT11 sensor
    if (dht_init(&dev, DHT_PIN) != 0) {
        printf("Error: Failed to initialize DHT11\n");
        return 1;
    }

    // Loop to read the sensor every 5 seconds
    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);
        } else {
            printf("Error: Failed to read from DHT11\n");
        }

        // Wait for the next interval
        ztimer_sleep(ZTIMER_MSEC, INTERVAL);
    }

    return 0;
}