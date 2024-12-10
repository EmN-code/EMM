#include <xtimer.h>
#include <stdio.h>
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN        GPIO_PIN(PORT_X, PIN_DHT)
#define LED_PIN        GPIO_PIN(PORT_Y, PIN_LED)
#define TEMP_THRESHOLD 25
#define SLEEP_INTERVAL (5U * US_PER_SEC)  // 5 seconds

int main(void) {
    if (gpio_init(LED_PIN, GPIO_OUT) < 0) {
        printf("Error initializing the LED pin\n");
        return -1;
    }

    // Initialize temperature sensor (Assuming you have a function to initialize, otherwise use specific DHT init code)
    dht_t dev;
    if (dht_init(&dev, dht_11) != 0) {
        printf("Failed to initialize DHT11 sensor\n");
        return -1;
    }

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Failed to read from DHT11 sensor\n");
        } else {
            printf("Temperature: %d, Humidity: %d\n", temp, hum);

            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Temperature threshold exceeded, LED ON\n");
            } else {
                gpio_clear(LED_PIN);
                printf("Temperature is normal, LED OFF\n");
            }
        }

        // Sleep for defined interval
        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}