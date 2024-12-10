#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 4) // GPIO pin number for DHT11
#define LED_PIN GPIO_PIN(0, 2) // GPIO pin number for LED
#define TEMP_THRESHOLD 30 // Temperature threshold for warning

int main(void) {
    // Initialize the GPIO pin for the LED
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN); // Make sure LED is initially off

    // Initialize the DHT11 sensor
    dht_t dev;
    dev.pin = DHT_PIN;
    dev.type = DHT11;
    
    int16_t temp, hum;

    while (1) {
        // Read data from the DHT11 sensor
        if (dht_read(&dev, &temp, &hum) != 0) {
            puts("[Error] Failed to read data from DHT11 sensor.");
            continue;
        }

        // Handle temperature data
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
               temp / 10, temp % 10, hum / 10, hum % 10);

        // Check if temperature exceeds threshold and toggle LED accordingly
        if (temp / 10 >= TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}