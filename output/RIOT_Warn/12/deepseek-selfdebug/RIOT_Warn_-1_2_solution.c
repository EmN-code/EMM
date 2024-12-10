
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN             GPIO_PIN(0, 18)  // GPIO pin for DHT11
#define LED_PIN             GPIO_PIN(0, 21)  // GPIO pin for LED
#define TEMP_THRESHOLD      25              // Temperature threshold in Celsius
#define INTERVAL            5               // Interval in seconds

static void read_dht11(void) {
    dht_params_t dht_params = {
        .pin = DHT_PIN,
        .type = DHT11,
        .in_mode = GPIO_IN_PU  // Use GPIO_IN_PU for pull-up mode
    };

    dht_t dev;
    int16_t temp, hum;

    if (dht_init(&dev, &dht_params) == DHT_OK) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);

            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            puts("Error reading DHT11 sensor.");
        }
    } else {
        puts("Error initializing DHT11 sensor.");
    }
}

int main(void) {
    // Initialize LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        read_dht11();
        xtimer_sleep(INTERVAL);
    }

    return 0;
}
