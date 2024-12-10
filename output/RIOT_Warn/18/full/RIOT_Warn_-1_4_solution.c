#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT11_PIN GPIO_PIN(PORT_A, 0) // Define the GPIO pin where DHT11 is connected
#define LED_PIN GPIO_PIN(PORT_A, 1)   // Define the GPIO pin where LED is connected

#define TEMPERATURE_THRESHOLD 30      // Define the temperature threshold in °C

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    // Initialize DHT11 sensor
    while (dht_init(&dev, DHT11_PIN) != 0) {
        printf("DHT11 initialization failed. Retrying...\n");
        xtimer_sleep(1);
    }

    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_set(LED_PIN);

    printf("System initialized.\n");

    while (1) {
        // Read temperature and humidity from the DHT11 sensor
        if (dht_read(&dev, &temp, &hum) != 0) {
            printf("Error reading DHT11\n");
        } else {
            printf("Temperature: %d °C, Humidity: %d %%\n", temp, hum);
        }

        // Check if temperature exceeds threshold
        if (temp > TEMPERATURE_THRESHOLD) {
            gpio_clear(LED_PIN); // Turn on LED
            printf("Warning! Temperature exceeded threshold.\n");
        } else {
            gpio_set(LED_PIN);   // Turn off LED
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}