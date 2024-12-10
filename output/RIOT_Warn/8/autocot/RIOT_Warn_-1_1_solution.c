#include "periph/gpio.h"
#include "dht.h"
#include "dht_params.h"
#include <xtimer.h>

#define LED_PIN GPIO_PIN(0, 2)  // Define the GPIO pin for the LED
#define TEMP_THRESHOLD 30       // Temperature threshold in degree Celsius

int main(void) {
    dht_t dev;
    dht_params_t params = DHT_PARAMS;
    params.pin = GPIO_PIN(0, 4);  // Define the GPIO pin connected to DHT11

    // Initialize the DHT11 sensor
    if (dht_init(&dev, &params) != 0) {
        puts("Error initializing DHT11 sensor\n");
        return -1;
    }

    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    int16_t temp, hum;

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temp, &hum) != 0) {
            puts("Error reading from DHT11\n");
        } else {
            // Convert temperature to degrees Celsius
            temp /= 10;
            printf("Temperature: %d°C, Humidity: %d%% \n", temp, hum/10);

            // Check if temperature exceeds the threshold
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                puts("Temperature threshold exceeded! LED ON\n");
            } else {
                gpio_clear(LED_PIN);
                puts("Temperature below threshold. LED OFF\n");
            }
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }
    return 0;
}