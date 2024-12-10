#include "ztimer.h"
#include "stdio.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT11_PIN GPIO_PIN(0, 2) // GPIO pin, port 0 pin 2
#define SLEEP_INTERVAL (5U * 1000U * 1000U) // 5 seconds in microseconds

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;
    printf("DHT11 example started\n");

    // Initialize the DHT11 sensor
    if (dht_init(&dev, DHT11_PIN) == DHT_OK) {
        printf("DHT11 sensor initialized successfully\n");
    } else {
        printf("Error initializing DHT11 sensor\n");
        return 1;
    }

    while (1) {
        // Read values from DHT11 sensor
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d°C, Humidity: %d%%\n", temperature / 10, humidity / 10);
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_USEC, SLEEP_INTERVAL);
    }

    return 0;
}