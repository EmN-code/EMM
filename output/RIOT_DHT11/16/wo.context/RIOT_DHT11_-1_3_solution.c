#include <stdio.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include "dht.h"

#define DHT11_PIN GPIO_PIN(0, 2)  // Assumes GPIO Port 0, Pin 2
#define DHT11_TYPE DHT11

int main(void)
{
    dht_t dev;
    int16_t temp, hum;
    
    // Initialize the DHT11 sensor
    if (dht_init(&dev, DHT11_PIN) != 0) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    while (1) {
        // Read values from DHT11 sensor
        if (dht_read(&dev, &temp, &hum) != 0) {
            printf("Failed to read from DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        }
        
        // Sleep for 5 seconds
        xtimer_sleep(5);
    }
    return 0;
}