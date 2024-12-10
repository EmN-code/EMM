#include "xtimer.h"
#include "dht.h"
#include "periph/gpio.h"

#define DHT_PIN GPIO_PIN(0, 4)  // Define the pin connected to DHT11
#define LED_PIN GPIO_PIN(0, 5)  // Define the pin connected to the LED
#define THRESHOLD_TEMPERATURE 30  // Temperature threshold
#define INTERVAL (5U * US_PER_SEC)  // 5 second interval

int main(void) {
    // Initialize the GPIO pin for the LED
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);  // Make sure LED is off initially

    // Initialize the DHT sensor
    dht_t dev;
    if (dht_init(&dev, &(dht_params_t){ .pin = DHT_PIN, .type = DHT11 }) != DHT_OK) {
        puts("Error initializing DHT11 sensor.\n");
        return 1;
    }

    int16_t temp, hum;

    while (1) {
        // Sleep for the interval duration
        xtimer_sleep(INTERVAL / US_PER_SEC);

        // Read temperature and humidity
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            puts("Error reading from DHT11 sensor.\n");
            continue;
        }

        // Print the temperature and humidity
        printf("Temperature: %d.%d°C\n", temp / 10, temp % 10);
        printf("Humidity: %d.%d%%\n", hum / 10, hum % 10);

        // Check if temperature exceeds the threshold
        if (temp / 10 > THRESHOLD_TEMPERATURE) {
            gpio_set(LED_PIN);  // Turn on LED
            puts("Temperature threshold exceeded! LED ON.\n");
        } else {
            gpio_clear(LED_PIN);  // Turn off LED
            puts("Temperature within limit. LED OFF.\n");
        }
    }

    return 0;
}