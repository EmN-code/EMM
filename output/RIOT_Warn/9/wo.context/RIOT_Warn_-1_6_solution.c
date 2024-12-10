#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 25)  // Assign a valid GPIO pin for DHT11
#define LED_PIN GPIO_PIN(0, 26)  // Assign a valid GPIO pin for the LED
#define TEMP_THRESHOLD 30        // Temperature threshold for LED warning (in Celsius)
#define DHT_TYPE DHT11

int main(void) {
    // Initialize GPIO
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        puts("Error: Could not initialize LED GPIO");
        return 1;
    }
    // Initially turn off the LED
    gpio_clear(LED_PIN);

    // DHT sensor initialization
    dht_t dev;
    int init = dht_init(&dev, DHT_PIN, DHT_TYPE);
    if (init != DHT_OK) {
        printf("Error: Could not initialize DHT11 sensor\n");
        return 1;
    }

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d°C, Humidity: %d%%\n", temp / 10, hum / 10);
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);  // Turn on LED if temperature exceeds threshold
            } else {
                gpio_clear(LED_PIN);  // Turn off LED
            }
        } else {
            printf("Error: Failed to read DHT11 sensor\n");
        }
        xtimer_sleep(5); // Sleep for 5 seconds
    }
    return 0;
}