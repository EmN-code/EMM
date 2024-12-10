#include <stdio.h>
#include <dht.h>
#include <periph/gpio.h>

#define DHT_PIN            GPIO_PIN(0, 4) // Example GPIO pin for DHT
#define LED_PIN            GPIO_PIN(0, 2) // Example GPIO pin for LED
#define TEMP_THRESHOLD     30 // Temperature threshold in degree Celsius

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;
    
    // Initialize DHT sensor
    dht_init(&dev, DHT_PIN);
    
    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%dC, Humidity: %d.%d%%\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);
            
            // Check if temperature exceeds threshold
            if (temperature / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN); // Turn on the LED
            } else {
                gpio_clear(LED_PIN); // Turn off the LED
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }

        ztimer_sleep(ZTIMER_MSEC, 5000); // Sleep for 5 seconds
    }

    return 0;
}