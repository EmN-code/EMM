// Description
// This code initializes an LED and a DHT11 sensor on an ESP32 using RIOT OS. It reads the temperature from the DHT11 every 5 seconds and turns on the LED if the temperature exceeds a certain threshold. The error was due to an undeclared 'PORT_B', which needs to be defined based on the GPIO pins for the ESP32.

// Required Imports
#include "xtimer.h"          // Provides timer functions
#include "periph/gpio.h"     // Provides GPIO functions
#include "dht.h"             // Provides functions to interface with DHT sensors

// Definitions
#define LED_PIN GPIO_PIN(0, 2)   // Define LED_PIN for ESP32, using GPIO2
#define DHT_PIN GPIO_PIN(0, 4)   // Define DHT_PIN for ESP32, using GPIO4
#define TEMP_THRESHOLD 30        // The temperature threshold

// Main function
enum {TIMER_INTERVAL = 5 * US_PER_SEC};   // Define timer interval

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT); // Initialize the LED pin as output
    dht_t dev;
    if (dht_init(&dev, DHT_PIN, DHT11) != DHT_OK) {
        printf("Error initializing DHT11\n");
        return 1;
    }
    int16_t temperature, humidity;
    while (1) {
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Error reading DHT11\n");
        }
        xtimer_sleep(TIMER_INTERVAL);
    }
    return 0;
}