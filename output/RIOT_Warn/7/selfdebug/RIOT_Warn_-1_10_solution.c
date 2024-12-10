#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_B, 5)    // Assume GPIO pin connected to DHT11 is pin 5 of PORT B
#define LED_PIN GPIO_PIN(PORT_B, 7)    // Assume GPIO pin connected to LED is pin 7 of PORT B
#define TEMP_THRESHOLD 30              // Temperature threshold in degrees Celsius

int main(void)
{
    dht_t dev;
    int16_t temp, hum;
    
    gpio_init(LED_PIN, GPIO_OUT);
    
    if (dht_init(&dev, DHT_PARAM_PIN, DHT_TYPE_DHT11) != DHT_SUCCESS) {
        printf("Failed to initialize DHT11\n");
        return 1;
    }

    while (1) {
        if (dht_measure(&dev, &temp, &hum) == DHT_SUCCESS) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp/10, temp%10, hum/10, hum%10);
            
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from DHT11\n");
        }
        
        xtimer_sleep(5);
    }

    return 0;
}