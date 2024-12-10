#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(0, 0)  // Example pin for DHT11
#define LED_PIN GPIO_PIN(0, 1)  // Example pin for LED
#define TEMP_THRESHOLD 30       // Temperature threshold in Celsius
#define INTERVAL_SECONDS 5      

dht_t dev;

int main(void) {
    // Initialize DHT11 sensor
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT11;
    if (dht_init(&dev, &params) != 0) {
        printf("Error initializing DHT11\n");
        return -1;
    }

    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    printf("DHT11 reader started\n");

    while (1) {
        int16_t temp, hum;
        // Read the temperature and humidity
        if (dht_read(&dev, &temp, &hum) != 0) {
            printf("Error reading DHT11\n");
        } else {
            // Temperature from DHT11 is in 10ths of a degree
            temp /= 10;
            printf("Temperature: %dC Humidity: %d%%\n", temp, hum);
            
            // Check if temperature exceeds threshold
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);  // Turn on LED
            } else {
                gpio_clear(LED_PIN);  // Turn off LED
            }
        }

        // Wait for next read
        xtimer_sleep(INTERVAL_SECONDS);
    }

    return 0;
}