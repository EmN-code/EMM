
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN             GPIO_PIN(0, 18)  // GPIO18 on ESP32
#define LED_PIN             GPIO_PIN(0, 2)   // GPIO2 on ESP32
#define TEMP_THRESHOLD      30               // Temperature threshold in Celsius
#define INTERVAL_MS         5000000          // 5 seconds in microseconds

static dht_t dht_dev;

void init_dht(void) {
    dht_params_t dht_params = {
        .pin = DHT_PIN,
        .type = DHT11
    };
    dht_init(&dht_dev, &dht_params);
}

void init_led(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);  // Ensure LED is off initially
}

int main(void) {
    int16_t temp, hum;

    init_dht();
    init_led();

    while (1) {
        if (dht_read(&dht_dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);

            if (temp > TEMP_THRESHOLD * 10) {
                gpio_set(LED_PIN);  // Turn on LED
            } else {
                gpio_clear(LED_PIN);  // Turn off LED
            }
        } else {
            puts("Failed to read from DHT sensor");
        }

        xtimer_usleep(INTERVAL_MS);
    }

    return 0;
}
