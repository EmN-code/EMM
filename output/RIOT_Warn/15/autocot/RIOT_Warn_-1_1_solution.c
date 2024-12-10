#include "xtimer.h"
#include "dht.h"
#include "thread.h"
#include "periph/gpio.h"

#define DHT_PIN     GPIO_PIN(PORT_A, 10)
#define LED_PIN     GPIO_PIN(PORT_B, 0)
#define TEMP_THRESHOLD 30 // Temperature threshold to trigger the LED in degrees Celsius
#define INTERVAL    (5U * US_PER_SEC) // 5-second interval in microseconds

int main(void) {
    dht_t dev;
    int16_t temp, hum;
    
    // Initialize DHT11 sensor
    if (dht_init(&dev, DHT_PIN, DHT11) != DHT_OK) {
        puts("Failed to initialize DHT11 sensor\n");
        return 1;
    }
    
    // Set LED pin to output mode
    gpio_init(LED_PIN, GPIO_OUT);
    
    while (1) {
        // Read temperature and humidity from DHT11
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d C, Humidity: %d.%d %%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            
            // Check if temperature exceeds threshold
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            puts("Failed to read from DHT11 sensor\n");
        }
        
        // Sleep for the interval period
        xtimer_sleep(INTERVAL / US_PER_SEC);
    }
    return 0;
}