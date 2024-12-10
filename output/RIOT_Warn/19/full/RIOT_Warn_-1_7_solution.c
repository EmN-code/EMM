#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <thread_config.h>
#include <periph/gpio.h>
#include <dht.h>  // Ensure DHT sensor library is available

#define DHT_PIN GPIO_PIN(PORT_A, 0)  // Define the pin according to your setup
#define LED_PIN GPIO_PIN(PORT_B, 0)  // Define the pin connected to the LED
#define THRESHOLD_TEMP 30  // Temperature threshold
#define INTERVAL (5U * US_PER_SEC)

dht_t dev;

int main(void) {
    // Initialize the DHT device
    if (dht_init(&dev, DHT_PIN, DHT11) != DHT_SUCCESS) {
        puts("Failed to initialize DHT sensor");
        return 1;
    }
    
    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        int16_t temp, hum;

        // Read data from the DHT sensor
        if (dht_read(&dev, &temp, &hum) == DHT_SUCCESS) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);

            // Check if temperature exceeds threshold
            if (temp / 10 > THRESHOLD_TEMP) {
                gpio_set(LED_PIN);
                puts("Temperature exceeded threshold, LED ON");
            } else {
                gpio_clear(LED_PIN);
                puts("Temperature is normal, LED OFF");
            }
        } else {
            puts("Failed to read data from DHT sensor");
        }

        // Wait for the next interval
        xtimer_sleep(INTERVAL);
    }
    return 0;
}