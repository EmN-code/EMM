#include <stdio.h>
#include "xtimer.h"
#include "dht.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "periph/i2c.h"

#define DHT_PIN GPIO_PIN(PA, 2)  // GPIO 2

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;
    int res;

    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_PIN, DHT_MODEL_DHT11) != DHT_OK) {
        puts("DHT initialization failed!");
        return 1;
    }

    puts("DHT initialization successful, starting reading...");
    
    while (1) {
        res = dht_read(&dev, &temperature, &humidity);
        // Check the return value and retry if it fails
        if (res != DHT_OK) {
            puts("Failed to read from DHT sensor!");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                   temperature / 10, temperature % 10, 
                   humidity / 10, humidity % 10);
        }
        
        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    return 0;
}