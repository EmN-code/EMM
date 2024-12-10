#include <stdio.h>
#include "timex.h"
#include "dht.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define DHT_PIN GPIO_PIN(PORT_A, 0)  // configure your specific port and pin
#define LED_PIN GPIO_PIN(PORT_B, 0)  // configure your specific port and pin
#define TEMP_THRESHOLD 30  // temperature threshold in Celsius
#define INTERVAL (5U * US_PER_SEC)  // 5 second interval

int main(void)
{
    dht_t dev;
    int16_t temp, hum;
    gpio_init(LED_PIN, GPIO_OUT);
    
    if (dht_init(&dev, DHT_TYPE_DHT11, DHT_PIN) != DHT_OK) {
        puts("Error initializing DHT11 sensor.");
        return 1;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            
            if ((temp / 10) > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                puts("Warning: Temperature threshold exceeded!");
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            puts("Failed to read data from DHT11 sensor.");
        }
        
        xtimer_sleep(INTERVAL);
    }
    
    return 0;
}