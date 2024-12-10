#include <xtimer.h>          // Timer functions
#include <stdio.h>           // Standard input-output library
#include <periph/gpio.h>     // GPIO functions
#include <dht.h>             // DHT sensor functions

#define DHT_PIN     GPIO_PIN(PORT_A, 0) // Use a valid GPIO pin for DHT
#define LED_PIN     GPIO_PIN(PORT_B, 0) // Use a valid GPIO pin for LED
#define TEMP_THRESHOLD 30   // Temperature threshold for LED warning
#define DELAY       (5U * US_PER_SEC) // 5 second delay

int main(void) {
    // Initialize the GPIO pins
    gpio_init(DHT_PIN, GPIO_IN);
    gpio_init(LED_PIN, GPIO_OUT);
    
    // DHT device descriptor
    dht_t dev;
    
    // Initialize the DHT device. Here DHT_TYPE_DHT11 is the type of sensor used
    if (dht_init(&dev, DHT_PIN, DHT_TYPE_DHT11) != DHT_OK) {
        printf("Failed to initialize DHT11 sensor!\n");
        return 1;
    }
    
    int16_t temperature, humidity;
    while (1) {
        if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
            printf("Failed to read from DHT11 sensor!\n");
        } else {
            printf("Temperature: %d.%dC\n", temperature / 10, temperature % 10);
            printf("Humidity: %d.%d\n", humidity / 10, humidity % 10);
            // Activate the LED if temperature exceeds threshold
            if (temperature / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        xtimer_sleep(5);
    }
    return 0;
}