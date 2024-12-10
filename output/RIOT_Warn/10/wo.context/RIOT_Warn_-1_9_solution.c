#include "periph/gpio.h"
#include "dht.h"
#include "xtimer.h"

#define DHT11_PARAM_PIN GPIO_PIN(0, 5) // Pin PA5, change as needed
#define LED_PIN GPIO_PIN(1, 1) // Example pin PB1, change as needed
#define THRESHOLD_TEMPERATURE 30

int main(void)
{
    // Initialize GPIOs
    gpio_init(LED_PIN, GPIO_OUT);
    dht_t dev;
    int16_t temperature, humidity;

    // Initialize the DHT sensor
    if (dht_init(&dev, DHT11_PARAM_PIN, DHT11) != DHT_OK) {
        puts("Failed to initialize DHT sensor");
        return 1;
    }

    while (1) {
        // Read values from DHT11 sensor
        if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
            puts("Failed to read DHT sensor");
            xtimer_sleep(5);
            continue;
        }

        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
               temperature / 10, temperature % 10, humidity / 10, humidity % 10);

        // Turn on LED if temperature exceeds threshold
        if (temperature / 10 > THRESHOLD_TEMPERATURE) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }

        // Sleep for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}