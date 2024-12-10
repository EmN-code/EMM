#include <stdio.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include "dht.h"

#define DHT_PARAM_PIN         GPIO_PIN(PORT, 5)   // Define the GPIO port for DHT
#define DHT_TYPE              DHT11
#define TEMP_THRESHOLD        30  // Temperature threshold for LED warning
#define SLEEP_INTERVAL_US     (5U * US_PER_SEC)  // 5 seconds interval
#define LED_PIN               GPIO_PIN(PORT, 6)  // GPIO Pin for LED

int main(void) {
    // Initialize DHT sensor
    dht_t dev;
    if (dht_init(&dev, DHT_PARAM_PIN, DHT_TYPE) != 0) {
        printf("Error initializing DHT sensor\n");
        return 1;
    }

    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    int16_t temperature, humidity;

    while (1) {
        // Read the DHT sensor values
        if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
            printf("Error reading DHT sensor\n");
        } else {
            printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);

            // Check temperature against threshold
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Warning: Temperature exceeds threshold!\n");
            } else {
                gpio_clear(LED_PIN);
            }
        }
        
        // Wait for the next cycle
        xtimer_sleep(SLEEP_INTERVAL_US / US_PER_SEC);
    }

    return 0;
}