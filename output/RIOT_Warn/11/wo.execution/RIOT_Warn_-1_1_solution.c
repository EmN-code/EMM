#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_B, 5) // DHT11 connected to pin B5
#define LED_PIN GPIO_PIN(PORT_C, 3) // LED connected to pin C3

#define TEMPERATURE_THRESHOLD 25 // Temperature threshold in degrees Celsius
#define SLEEP_INTERVAL (5U * US_PER_SEC) // Sleep interval of 5 seconds

int main(void) {
    dht_t dev;
    dht_params_t params;

    // Initialize DHT11 sensor
    params.pin = DHT_PIN;
    dht_init(&dev, &params);

    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        int16_t temp, hum;

        // Read temperature and humidity from DHT11
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d°C, Humidity: %d%%\n", temp, hum);

            // Check if temperature exceeds threshold
            if (temp > TEMPERATURE_THRESHOLD) {
                gpio_set(LED_PIN); // Turn on LED
            } else {
                gpio_clear(LED_PIN); // Turn off LED
            }
        } else {
            puts("Error reading from DHT11 sensor");
        }

        // Sleep for the defined interval
        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}