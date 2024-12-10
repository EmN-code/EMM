
#include "periph/gpio.h"
#include "xtimer.h"
#include "dht.h"
#include "stdio.h"

#define DHT_PIN             GPIO_PIN(0, 18)  // GPIO pin for DHT11
#define LED_PIN             GPIO_PIN(0, 2)   // GPIO pin for LED
#define TEMP_THRESHOLD      25               // Temperature threshold in Celsius
#define INTERVAL_MS         5000000          // 5 seconds interval in microseconds

static void init_dht(dht_t *dev, gpio_t pin) {
    dht_params_t params = {
        .pin = pin,
        .type = DHT11
    };
    dht_init(dev, &params);
}

static void init_led(gpio_t pin) {
    gpio_init(pin, GPIO_OUT);
    gpio_set(pin);  // Turn off LED initially
}

int main(void) {
    dht_t dht_dev;
    int16_t temp, hum;
    xtimer_ticks32_t last_wakeup = xtimer_now();

    init_dht(&dht_dev, DHT_PIN);
    init_led(LED_PIN);

    while (1) {
        if (dht_read(&dht_dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);

            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_clear(LED_PIN);  // Turn on LED
            } else {
                gpio_set(LED_PIN);    // Turn off LED
            }
        } else {
            puts("Failed to read from DHT sensor");
        }

        xtimer_periodic_wakeup(&last_wakeup, INTERVAL_MS);
    }

    return 0;
}
