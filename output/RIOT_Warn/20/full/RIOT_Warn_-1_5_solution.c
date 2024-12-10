#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include "dht.h"

#define DHT_PIN         GPIO_PIN(PORT_A, 2)  // Ensure the correct PORT and PIN
#define LED_PIN         GPIO_PIN(PORT_B, 5)  // Ensure the correct PORT and PIN
#define TEMP_THRESHOLD  30                   // Threshold temperature in Celsius
#define DHT_TYPE        DHT11                // DHT11 sensor type

static dht_t dev;

int main(void) {
    printf("Starting the DHT11 temperature reading application...\n");
    
    // Initialize GPIO for LED
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error initializing GPIO for LED\n");
        return 1;
    }
    gpio_clear(LED_PIN);

    // Initialize the DHT device
    if (dht_init(&dev, DHT_PIN, DHT_TYPE) != DHT_OK) {
        printf("Error initializing DHT device\n");
        return 1;
    }

    int16_t temp, hum;
    while (1) {
        int result = dht_read(&dev, &temp, &hum);
        if (result == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                temp / 10, temp % 10, hum / 10, hum % 10);

            // If temperature exceeds the threshold, turn on the LED
            if (temp > TEMP_THRESHOLD * 10) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from DHT sensor. Retrying...\n");
        }

        // Wait 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}