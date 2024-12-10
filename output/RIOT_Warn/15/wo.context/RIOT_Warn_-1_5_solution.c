#include <xtimer.h>
#include <periph/gpio.h>
#include <dht.h>
#include <stdio.h>

#define DHT_PIN         GPIO_PIN(PORT_A, 1)   // Assuming PORT_A needs to be correctly defined in RIOT
#define LED_PIN         GPIO_PIN(PORT_B, 2)   // Assuming PORT_B needs to be correctly defined too
#define TEMP_THRESHOLD  30                    // Example threshold temperature

int main(void) {
    // Initialize the DHT sensor
    dht_t dev;
    int16_t temp, hum;

    // Initialize LED
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error initializing LED\n");
        return 1;
    }

    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_PIN, DHT11) != 0) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    while (1) {
        // Read values from DHT
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Error reading from DHT sensor\n");
        } else {
            printf("Temperature: %d Humidity: %d\n", temp, hum);
            // Check temperature and control LED
            if (temp >= TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        xtimer_sleep(5); // Wait for 5 seconds
    }
    return 0;
}