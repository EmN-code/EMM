#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(0, 2)
#define DHT_TYPE DHT11

int main(void) {
    dht_t dev;
    int16_t temp, hum;
    
    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_TYPE) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    while (1) {
        // Read values from the DHT sensor
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Failed to read data from DHT sensor\n");
        }
        else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                    temp / 10, temp % 10, hum / 10, hum % 10);
        }
        
        // Sleep for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}