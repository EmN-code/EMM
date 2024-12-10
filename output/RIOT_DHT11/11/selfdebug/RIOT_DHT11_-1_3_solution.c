#include <xtimer.h>
#include <stdio.h>
#include "dht.h"
#include "periph/gpio.h"

#define DHT_PIN GPIO_PIN(0, 2)  // GPIO 2 is connected to port 0 pin 2 for ESP32
#define DHT_TYPE DHT11         // Using DHT11 sensor type
#define SLEEP_TIME 5 * XTIMER_SEC // Sleep time of 5 seconds

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;
    
    if (dht_init(&dev, DHT_TYPE, DHT_PIN) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                temperature / 10, temperature % 10, 
                humidity / 10, humidity % 10);
        } else {
            printf("Failed to read data from sensor\n");
        }
        xtimer_sleep(SLEEP_TIME);
    }

    return 0;
}