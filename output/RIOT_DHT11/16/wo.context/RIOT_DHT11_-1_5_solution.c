#include "xtimer.h"
#include "dht.h"
#include "gpio.h"
#include "stdio.h"

#define DHT_PIN GPIO_PIN(0, 2) // GPIO pin 2

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    // Initialize the GPIO pin as a DHT device
    if (dht_init(&dev, DHT_PIN) != DHT_OK) {
        printf("Error initializing DHT sensor\n");
        return 1;
    }

    while (1) {
        // Read values from the DHT sensor
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Error reading DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        }

        // Wait for 5 seconds before reading again
        xtimer_sleep(5);
    }

    return 0;
}