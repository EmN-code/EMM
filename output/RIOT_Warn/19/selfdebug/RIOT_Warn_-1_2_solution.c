#include <xtimer.h>
#include <stdlib.h>
#include <stdio.h>
#include <periph/gpio.h>

#define DHT_TRIGGER_INTERVAL (5U * 1000000U) // 5 seconds
#define TEMP_THRESHOLD 30 // Temperature threshold in Celsius
#define LED_PIN GPIO_PIN(1, 0) // Define your GPIO pin for LED, varies by board

int main(void) {
    dht_t dev;
    int16_t temp, hum;
    
    // Initialize the DHT sensor (assuming PORT_B and pin 0)
    if (dht_init(&dev, GPIO_PIN(PORT_B, 0)) != 0) {
        printf("Error: Could not initialize DHT sensor\n");
        return 1;
    }
    
    // Initialize LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);
    
    while (1) {
        // Read from DHT sensor
        if (dht_read(&dev, &temp, &hum) != 0) {
            printf("Error: Could not read data from DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            // Check temperature threshold
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        // Wait for the next trigger interval
        xtimer_usleep(DHT_TRIGGER_INTERVAL);
    }
    return 0;
}