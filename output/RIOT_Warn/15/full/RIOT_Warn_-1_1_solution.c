#include <xtimer.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 0) // Specify the pin where DHT is connected
#define LED_PIN GPIO_PIN(PORT_B, 0) // Specify the pin where LED is connected
#define TEMP_THRESHOLD 30          // Temperature threshold for LED warning
#define INTERVAL (5U * 1000000U)   // 5-second interval in microseconds

int main(void) {
    // Initialize DHT sensor
    dht_t dev;
    dht_params_t params = { .pin = DHT_PIN, .type = DHT11 };
    if (dht_init(&dev, &params) != DHT_OK) {
        printf("Error initializing DHT sensor\n");
        return 1;
    }
   
    // Set LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);
    
    while (1) {
        int16_t temp, hum;
        
        // Read temperature and humidity
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Failed to read from DHT sensor\n");
        }
        else {
            // Convert temperature to a human-readable format
            temp /= 10;
            printf("Temperature: %d°C, Humidity: %d%%\n", temp, hum);
            
            // Check if temperature exceeds the threshold
            if (temp >= TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Temperature exceeds threshold! LED is ON.\n");
            }
            else {
                gpio_clear(LED_PIN);
                printf("Temperature is below threshold. LED is OFF.\n");
            }
        }
        
        // Sleep for the set interval
        xtimer_usleep(INTERVAL);
    }
    return 0;
}