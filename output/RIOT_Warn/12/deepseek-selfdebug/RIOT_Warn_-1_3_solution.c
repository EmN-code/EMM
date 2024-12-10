
#include <stdio.h>
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN             GPIO_PIN(0, 13)  // GPIO13 on ESP32
#define LED_PIN             GPIO_PIN(0, 2)   // GPIO2 on ESP32
#define TEMP_THRESHOLD      30               // Temperature threshold in Celsius
#define INTERVAL            5                // Interval in seconds

void read_dht11(void) {
    dht_t dev;
    int16_t temp, hum;

    if (dht_init(&dev, DHT_PIN) == DHT_OK) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);

            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            puts("Failed to read from DHT11 sensor");
        }
    } else {
        puts("Failed to initialize DHT11 sensor");
    }
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the xtimer
    xtimer_init();

    while (1) {
        read_dht11();
        xtimer_sleep(INTERVAL);
    }

    return 0;
}
