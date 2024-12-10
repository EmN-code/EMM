#include <xtimer.h>
#include <stdio.h>
#include <dht.h>
#include <periph/gpio.h>

#define DHT_PIN GPIO_PIN(PORT_A, 0) // Define the correct DHT pin number
#define LED_PIN GPIO_PIN(PORT_A, 1) // Define the correct LED pin number
#define TEMP_THRESHOLD 30.0 // Temperature threshold in Celsius

static dht_t dev;

int main(void)
{
    // Initialize DHT sensor
    if (dht_init(&dev, DHT_PIN) != 0) {
        puts("Failed to initialize DHT sensor.");
        return 1;
    }

    // Configure LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    int16_t temp, hum;
    while (1) {
        // Read DHT sensor values
        if (dht_read(&dev, &temp, &hum) != 0) {
            puts("Failed to read from DHT sensor.");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp/10, temp%10, hum/10, hum%10);

            // Check if temperature exceeds the threshold
            if (temp/10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN); // Turn on LED
            } else {
                gpio_clear(LED_PIN); // Turn off LED
            }
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}