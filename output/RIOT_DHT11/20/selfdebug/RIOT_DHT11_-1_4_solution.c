#include <xtimer.h>
#include <stdio.h>
#include <dht.h>
#include <periph/gpio.h>

#define DHT_PIN GPIO_PIN(0, 2) // GPIO 2

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    // Initialize the DHT device
    if (dht_init(&dev, DHT_PIN, DHT11) != 0) {
        printf("Failed to initialize DHT11\n");
        return 1;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) != 0) {
            printf("Error reading data from DHT11\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        }
        xtimer_sleep(5); // sleep for 5 seconds
    }
    return 0;
}