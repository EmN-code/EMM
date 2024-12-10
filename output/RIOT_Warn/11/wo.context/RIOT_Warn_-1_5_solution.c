#include <xtimer.h>
#include <timex.h>
#include <stdio.h>
#include <periph/gpio.h>

#include "dht.h"

#define LED_PIN             GPIO_PIN(PORT_B, 0) // Make sure to use the correct GPIO port and pin for your specific board
#define DHT_NAME            DHT11
#define THRESHOLD_TEMP      30 // Temperature threshold to trigger LED
#define SLEEP_INTERVAL      (5U * US_PER_SEC) // 5 seconds

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    // Initialize the GPIO pin for the LED
    if (gpio_init(LED_PIN, GPIO_OUT) < 0) {
        printf("Error: Unable to initialize the LED GPIO\n");
        return 1;
    }

    // Initialize the DHT sensor on a designated GPIO pin
    if (dht_init(&dev, GPIO_PIN(PORT_A, 5), DHT_NAME) != DHT_OK) {  // Make sure to use the correct GPIO port and pin
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);

            if (temp / 10 > THRESHOLD_TEMP) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }

        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}