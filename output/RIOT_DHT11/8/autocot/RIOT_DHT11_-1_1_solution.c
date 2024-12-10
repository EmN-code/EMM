#include <stdio.h>
#include <dht.h>
#include <xtimer.h>

#define GPIO_PIN 2  // GPIO pin where the DHT11 is connected
#define INTERVAL (5U * 1000000U)  // 5 seconds interval in microseconds

dht_t dev;
dht_params_t params = {
    .pin = GPIO_PIN,
    .type = DHT_TYPE_DHT11
};

int main(void) {
    int16_t temperature, humidity;

    // Initialize the DHT11 sensor
    if (dht_init(&dev, &params) != DHT_OK) {
        printf("Failed to initialize DHT11 sensor\n");
        return 1;
    }

    // Main loop
    while (1) {
        // Read temperature and humidity from the DHT11 sensor
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%dC, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }

        // Wait for the defined interval
        xtimer_usleep(INTERVAL);
    }

    return 0;
}