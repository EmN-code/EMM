#include <xtimer.h>
#include <stdio.h>
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 0)  // Adjust based on your setup
#define LED_PIN GPIO_PIN(PORT_B, 0)  // Adjust based on your setup
#define TEMP_THRESHOLD 30  // Temperature threshold for LED warning

int main(void) {
    // Initialize the GPIO pin for the LED
    gpio_init(LED_PIN, GPIO_OUT);
    // Initialize the DHT sensor
    dht_t dev;
    int16_t temp, hum;
    
    // Continuous loop to read temperature and control LED
    while (1) {
        // Read data from DHT11 sensor
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", (temp / 10), (temp % 10), (hum / 10), (hum % 10));
            
            // Check if temperature exceeds the threshold
            if (temp > TEMP_THRESHOLD * 10) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read data from DHT11 sensor\n");
        }
        
        // Wait for 5 seconds before the next read
        xtimer_sleep(5);
    }
    
    return 0;
}