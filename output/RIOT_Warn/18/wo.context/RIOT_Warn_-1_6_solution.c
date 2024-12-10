#include <dht.h>
#include <xtimer.h>
#include <stdio.h>

#define DHT_PIN GPIO_PIN(PORT_A, 0)  // Change according to your setup
#define LED_PIN GPIO_PIN(PORT_B, 3)  // Change according to your setup
#define THRESHOLD_TEMPERATURE 30     // Set your temperature threshold

int main(void) {
    // Initialize the DHT sensor
    dht_t dev;
    int16_t temp, hum;

    if (dht_init(&dev, DHT_PIN) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        // Read data from the DHT11 sensor
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Failed to read from DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            // Check if temperature exceeds the threshold
            if (temp / 10 > THRESHOLD_TEMPERATURE) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        xtimer_sleep(5);
    }

    return 0;
}