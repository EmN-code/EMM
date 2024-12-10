#include <ztimer.h>
#include <stddef.h>
#include <stdlib.h>
#include "dht.h"
#include "esp_system.h"
#include "periph/gpio.h"

#define TEMP_THRESHOLD 30 // Example temperature threshold
#define DHT_PIN GPIO_PIN(0) // Example GPIO for DHT
#define LED_PIN GPIO_PIN(2) // Example GPIO for LED

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    // Initialize the DHT sensor
    if (dht_init(&dev, DHT11, DHT_PIN) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        // Read temperature and humidity from the DHT sensor
        if(dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Error reading DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d\%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
            
            // Check if temperature exceeds threshold
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN); // Turn the LED on
            } else {
                gpio_clear(LED_PIN); // Turn the LED off
            }
        }

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return 0;
}
