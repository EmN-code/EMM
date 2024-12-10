#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(0, 5) // Example GPIO pin
#define LED_PIN GPIO_PIN(0, 2) // Example GPIO pin
#define TEMP_THRESHOLD 30 // Temperature threshold

void read_dht_temperature(void) {
    int16_t temperature, humidity;
    dht_t dev;

    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_PIN, DHT11) != 0) {
        printf("Failed to initialize DHT sensor\n");
        return;
    }

    while (1) {
        int res = dht_read(&dev, &temperature, &humidity);
        if (res != DHT_OK) {
            printf("Failed to read data from sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);

            // Check if temperature exceeds threshold
            if ((temperature / 10) > TEMP_THRESHOLD) {
                gpio_set(LED_PIN); // Turn on LED
            } else {
                gpio_clear(LED_PIN); // Turn off LED
            }
        }

        xtimer_sleep(5); // Delay 5 seconds
    }
}

int main(void) {
    // Initialize the main application logic
    read_dht_temperature();
    return 0;
}