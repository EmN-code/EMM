#include "xtimer.h"
#include "thread.h"
#include "periph/gpio.h"
#include "dht.h"

#define LED_PIN GPIO_PIN(0, 2) // Change according to your setup
#define TEMP_THRESHOLD 30      // Temperature threshold for LED warning
#define DELAY 5                // Delay in seconds

int main(void) {
    // Initialize the DHT sensor (example DHT11 on GPIO_PIN 0,0)
    dht_t dev;
    dht_params_t params = {
        .type = DHT11,
        .pin = GPIO_PIN(0, 0)
    };

    if (dht_init(&dev, &params) != 0) {
        puts("DHT initialization failed");
        return 1;
    }

    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    int16_t temp, hum;

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temp, &hum) != 0) {
            puts("Failed to read data from sensor");
        } else {
            printf("Temperature: %d, Humidity: %d\n", temp, hum);

            // Check temperature and turn on LED if it exceeds threshold
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                puts("WARNING: Temperature threshold exceeded");
            } else {
                gpio_clear(LED_PIN);
            }
        }

        // Wait for the defined delay
        xtimer_sleep(DELAY);
    }

    return 0;
}
