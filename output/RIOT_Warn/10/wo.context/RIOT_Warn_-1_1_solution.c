#include <ztimer.h>  // Timer management header
#include <stdio.h>   // Standard I/O header
#include <periph/gpio.h>  // GPIO peripheral interface
#include <dht.h>  // DHT sensor library

#define DHT_PIN GPIO_PIN(PORT_A, 0)  // Define DHT11 data pin
#define LED_PIN GPIO_PIN(PORT_B, 1)  // Define LED pin
#define TEMP_THRESHOLD 30  // Define temperature threshold in Celsius

static dht_t dht_dev;  // DHT sensor device structure

int main(void) {
    int16_t temperature, humidity;
    int res;

    // Initialize DHT11 sensor
    dht_params_t dht_params;
    dht_params.pin = DHT_PIN;
    dht_params.type = DHT11;
    res = dht_init(&dht_dev, &dht_params);
    if (res != DHT_OK) {
        printf("Error initializing DHT11\n");
        return 1;
    }

    // Initialize LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        // Read temperature and humidity from DHT11
        res = dht_read(&dht_dev, &temperature, &humidity);
        if (res != DHT_OK) {
            printf("Error reading DHT11\n");
        } else {
            printf("Temperature: %d.%d C\n", temperature / 10, temperature % 10);
            printf("Humidity: %d.%d %%\n", humidity / 10, humidity % 10);

            // Turn on LED if temperature exceeds the threshold
            if (temperature > TEMP_THRESHOLD * 10) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }

        // Wait for 5 seconds
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}