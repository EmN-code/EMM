#include <xtimer.h>
#include <stdio.h>
#include <dht.h>
#include <periph/gpio.h>

#define DHT11_PIN GPIO_PIN(0, 2)
#define INTERVAL 5

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    if (dht_init(&dev, DHT11_PIN) != DHT_OK) {
        printf("Failed to initialize DHT11 sensor\n");
        return 1;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read data from DHT11 sensor\n");
        }
        
        // Sleep for the defined interval before reading again
        xtimer_sleep(INTERVAL);
    }

    return 0;
}