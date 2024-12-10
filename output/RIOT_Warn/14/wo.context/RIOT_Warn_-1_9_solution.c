#include <xtimer.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(PORT_A, 0)  // Adjust according to your setup
#define LED_PIN GPIO_PIN(PORT_B, 1)  // Adjust according to your setup
#define TEMP_THRESHOLD 30  // Temperature threshold for LED warning

int main(void) {
    // Initialize the DHT sensor
    dht_t dev;
    if (dht_init(&dev, DHT_PIN, DHT11) != DHT_OK) {
        puts("Failed to initialize DHT sensor");
        return 1;
    }

    // Initialize LED pin as an output
    gpio_init(LED_PIN, GPIO_OUT);
    
    while(1) {
        // Read the temperature
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            puts("Failed to read from DHT sensor");
        } else {
            // Convert temperature to Celsius
            temp /= 10;
            printf("Temperature: %d°C, Humidity: %d%%\n", temp, hum / 10);

            // Turn on LED if temperature exceeds threshold
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                puts("Warning: Temperature threshold exceeded!");
            } else {
                gpio_clear(LED_PIN);
            }
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}