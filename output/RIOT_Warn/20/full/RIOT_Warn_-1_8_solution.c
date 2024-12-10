#include <xtimer.h>
#include <periph/gpio.h>
#include <dht.h>
#include <stdio.h>

#define LED_PIN GPIO_PIN(0, 17) // Assume GPIO 17 as LED
#define DHT_PIN GPIO_PIN(0, 2)  // Assume GPIO 2 for DHT11
#define TEMP_THRESHOLD 25       // Temperature threshold in Celsius

int main(void)
{
    gpio_init(LED_PIN, GPIO_OUT);
    dht_t dev;
    int16_t temperature, humidity;
    
    if (dht_init(&dev, DHT_PIN, DHT11) != DHT_OK) {
        printf("DHT init failed\n");
        return 1;
    }

    while (1) {
        // Read data from DHT11
        if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
            printf("DHT read failed\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);
            
            if (temperature / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }
    return 0;
}