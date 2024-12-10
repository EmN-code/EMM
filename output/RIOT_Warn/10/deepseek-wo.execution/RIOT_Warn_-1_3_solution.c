
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"
#include "fmt.h"

#define DHT_PIN             GPIO_PIN(0, 18)  // Example GPIO pin for DHT11
#define LED_PIN             GPIO_PIN(0, 21)  // Example GPIO pin for LED
#define TEMP_THRESHOLD      30               // Temperature threshold in Celsius
#define SLEEP_INTERVAL      5                // Sleep interval in seconds

static dht_t dht_dev;

void init_dht(void) {
    dht_params_t dht_params = {
        .pin = DHT_PIN,
        .type = DHT11,
    };
    dht_init(&dht_dev, &dht_params);
}

void init_led(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_set(LED_PIN);  // Turn off LED initially
}

int main(void) {
    int16_t temperature, humidity;

    init_dht();
    init_led();

    while (1) {
        if (dht_read(&dht_dev, &temperature, &humidity) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);

            if (temperature > TEMP_THRESHOLD * 10) {
                gpio_clear(LED_PIN);  // Turn on LED
                puts("Temperature exceeds threshold!");
            } else {
                gpio_set(LED_PIN);  // Turn off LED
            }
        } else {
            puts("Failed to read from DHT sensor");
        }

        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}
