#include <stdio.h>
#include <unistd.h>
#include "xtimer.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 5)  // Adjust according to your wiring
#define LED_PIN GPIO_PIN(0, 2)  // Adjust according to your wiring
#define TEMP_THRESHOLD 30       // Temperature threshold in degree Celsius
#define DHT_TYPE DHT11

int main(void) {
    dht_t dev;
    gpio_init(LED_PIN, GPIO_OUT);

    if (dht_init(&dev, &DHT_TYPE, DHT_PIN) != 0) {
        printf("DHT initialization failed\n");
        return 1;
    }
    
    while (1) {
        int16_t temperature, humidity;
        if (dht_read(&dev, &temperature, &humidity) != 0) {
            printf("Error reading DHT sensor\n");
        } else {
            printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Warning: Temperature too high!\n");
            } else {
                gpio_clear(LED_PIN);
            }
        }
        xtimer_sleep(5);
    }
    
    return 0;
}
