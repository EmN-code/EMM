#include "thread.h"
#include "event/thread.h"
#include "dht.h"
#include "dht_params.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "mma8x5x_regs.h"
#include "periph/gpio.h"
#include <ztimer.h>
#include <stdio.h>

#define DHT_PIN GPIO_PIN(PORT_A, 0)  // Configure this to your specific setup
#define LED_PIN GPIO_PIN(PORT_A, 1)  // Configure this to your specific setup
#define TEMP_THRESHOLD 30  // Temperature threshold in Celsius

static dht_t dht_dev;
static dht_params_t dht_params = {
    .pin = DHT_PIN,
    .type = DHT11
};

int main(void) {
    // Initialize DHT sensor
    if (dht_init(&dht_dev, &dht_params) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    // Configure LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        int16_t temperature = 0;
        int16_t humidity = 0;

        if (dht_read(&dht_dev, &temperature, &humidity) != DHT_OK) {
            printf("Failed to read from DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);

            // Check temperature threshold
            if (temperature / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return 0;
}