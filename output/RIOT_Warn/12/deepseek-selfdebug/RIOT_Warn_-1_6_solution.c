
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "dht.h"
#include "xtimer.h"

#define DHT_PIN             GPIO_PIN(0, 4)  // GPIO4 on ESP32
#define LED_PIN             GPIO_PIN(0, 2)  // GPIO2 on ESP32
#define TEMP_THRESHOLD      30              // Temperature threshold in Celsius
#define INTERVAL_MS         5000000         // 5 seconds in microseconds

dht_t dht_dev;

void init_dht(void) {
    dht_params_t dht_params = {
        .pin = DHT_PIN,
        .type = DHT11
    };
    dht_init(&dht_dev, &dht_params);
}

void init_led(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);
}

int main(void) {
    int16_t temperature, humidity;

    init_dht();
    init_led();

    while (1) {
        if (dht_read(&dht_dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);

            if (temperature > TEMP_THRESHOLD * 10) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }

        xtimer_usleep(INTERVAL_MS);
    }

    return 0;
}
