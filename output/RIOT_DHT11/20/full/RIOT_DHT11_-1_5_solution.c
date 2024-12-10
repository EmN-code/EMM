#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(0, 2)  // GPIO pin where the sensor is connected
#define DHT_TYPE DHT11          // Sensor type DHT11
#define INTERVAL (5U * US_PER_SEC)  // Interval of 5 seconds

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;
    int result;

    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_TYPE, DHT_PIN) != DHT_OK) {
        puts("Failed to initialize DHT sensor");
        return 1;
    }

    // Main loop
    while (1) {
        result = dht_read(&dev, &temperature, &humidity);
        if (result == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        } else {
            puts("Failed to read from DHT sensor");
        }

        xtimer_sleep(INTERVAL / US_PER_SEC);  // Sleep for 5 seconds
    }

    return 0;
}