#include <stdio.h>
#include <xtimer.h>
#include <dht.h>
#include <periph/gpio.h>

#define DHT_PIN 2 // assuming GPIO pin 2 for DHT11, change as per your configuration
#define LED_PIN 15 // assuming GPIO pin 15 for the LED, change as per your configuration
#define TEMPERATURE_THRESHOLD 30 // temperature threshold in Celsius
#define DELAY 5 // delay in seconds

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;
    dht_params_t params;

    // Initialize DHT sensor
    params.pin = DHT_PIN;
    params.type = DHT11;
    if (dht_init(&dev, &params) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    // Initialize LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    printf("Starting main loop\n");
    while (1) {
        // Read temperature and humidity values
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%d C, Humidity: %d.%d %%RH\n", 
                   temperature / 10, temperature % 10, 
                   humidity / 10, humidity % 10);
            
            // Check if temperature exceeds the threshold
            if (temperature / 10 > TEMPERATURE_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Temperature exceeds threshold! Turning on LED.\n");
            } else {
                gpio_clear(LED_PIN);
                printf("Temperature is normal.\n");
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }

        // Wait for the next cycle
        xtimer_sleep(DELAY);
    }

    return 0;
}