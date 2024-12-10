
#include <stdio.h>
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "dht.h"
#include "event/thread.h"
#include "dht_params.h"
#include "thread.h"

#define DHT_PIN             GPIO_PIN(0, 18)
#define LED_PIN             GPIO_PIN(0, 2)
#define TEMP_THRESHOLD      30

static void _gpio_toggle(gpio_t pin) {
    gpio_toggle(pin);
}

int main(void) {
    int16_t temperature, humidity;
    dht_t dht_dev;

    // Initialize the DHT11 sensor
    dht_dev.pin = DHT_PIN;
    dht_dev.type = DHT_TYPE_DHT11;

    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    while (1) {
        // Read temperature and humidity from the DHT11 sensor
        if (dht_read(&dht_dev, &temperature, &humidity) == 0) {
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);

            // Check if the temperature exceeds the threshold
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }

        // Wait for 5 seconds before the next reading
        xtimer_sleep(5);
    }

    return 0;
}
