#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xtimer.h"
#include "dht.h"
#include "periph/gpio.h"

#define DHT_PIN GPIO_PIN(PORT_A, 0) // Adjust PIN as necessary
#define LED_PIN GPIO_PIN(PORT_B, 0) // Adjust PIN as necessary
#define TEMP_THRESHOLD 30 // Temperature threshold to trigger LED
#define INTERVAL 5 * US_PER_SEC // Read interval: 5 seconds

int main(void) {
    // Initialize GPIO for DHT and LED
    gpio_init(DHT_PIN, GPIO_IN);
    gpio_init(LED_PIN, GPIO_OUT);

    dht_t dev;
    int16_t temp, hum;
    
    // Initialize the DHT parameters
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT11;
    dht_init(&dev, &params);

    xtimer_ticks32_t last = xtimer_now();

    while (1) {
        // Read values from DHT11
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C
", temp / 10, temp % 10);
            printf("Humidity: %d.%d%%
", hum / 10, hum % 10);

            // If temperature is above threshold, turn on LED
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }

        // Wait for the next interval
        xtimer_periodic_wakeup(&last, INTERVAL);
    }
    return 0;
}