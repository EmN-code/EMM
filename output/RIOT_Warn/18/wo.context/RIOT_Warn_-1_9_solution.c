#include <stdio.h>
#include <stdlib.h>
#include <dht.h>
#include <xtimer.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(PORT_C, 3)  // Adjust according to your actual setup
#define DHT_PIN GPIO_PIN(0, 5)       // Example pin, change according to your setup
#define THRESHOLD_TEMPERATURE 30     // Temperature threshold
#define SLEEP_DURATION (5U * US_PER_SEC) // Sleep duration in microseconds (5 seconds)

dht_t dev;

static void read_temperature(void) {
    int16_t temperature, humidity;
    if (dht_read(&dev, DHT_PIN) == DHT_OK) {
        temperature = dht_get_temperature(&dev);
        humidity = dht_get_humidity(&dev);
        printf("Temperature: %d.%d°C, Humidity: %d.%d\n",
               temperature / 10, temperature % 10, humidity / 10, humidity % 10);
        if (temperature > THRESHOLD_TEMPERATURE * 10) {  // Convert to dht scale
            gpio_set(LED_PIN);
            printf("Warning: High temperature!\n");
        } else {
            gpio_clear(LED_PIN);
        }
    } else {
        printf("Error reading from sensor\n");
    }
}

int main(void) {
    // Initialize GPIO for LED
    gpio_init(LED_PIN, GPIO_OUT);
    // Initialize DHT on specified pin
    if (dht_init(&dev, DHT_PIN, DHT11) != DHT_OK) {
        printf("Failed to initialize DHT11\n");
        return EXIT_FAILURE;
    }

    while (1) {
        read_temperature();
        xtimer_usleep(SLEEP_DURATION);
    }

    return 0;
}