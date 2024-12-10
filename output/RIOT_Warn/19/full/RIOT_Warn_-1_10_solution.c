#include <xtimer.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(PORT_A, 0) // Replace 'PORT_A' and '0' with actual pin configuration
#define LED_PIN GPIO_PIN(PORT_B, 1) // Replace 'PORT_B' and '1' with actual pin configuration
#define TEMP_THRESHOLD 30.0

int main(void) {
    // Initialize DHT sensor
    dht_t dev;
    if (dht_init(&dev, DHT_PIN) != DHT_OK) {
        puts("Failed to initialize DHT sensor");
        return 1;
    }

    // Initialize LED pin
    if (gpio_init(LED_PIN, GPIO_OUT) != 0) {
        puts("Error initializing LED pin");
        return 1;
    }

    while (1) {
        int16_t temperature, humidity;
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);

            // Check temperature threshold
            if ((temperature / 10.0) > TEMP_THRESHOLD) {
                gpio_set(LED_PIN); // Turn on LED
            } else {
                gpio_clear(LED_PIN); // Turn off LED
            }
        } else {
            puts("Failed to read from DHT sensor");
        }

        xtimer_sleep(5); // Sleep for 5 seconds
    }

    return 0;
}