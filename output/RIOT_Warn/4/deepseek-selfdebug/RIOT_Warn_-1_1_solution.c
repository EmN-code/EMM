
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"
#include "thread.h"

#define DHT_PIN             GPIO_PIN(0, 18)  // GPIO pin for DHT11
#define LED_PIN             GPIO_PIN(0, 2)   // GPIO pin for LED
#define TEMP_THRESHOLD      25               // Temperature threshold in Celsius
#define SLEEP_INTERVAL      5                // Sleep interval in seconds

static dht_t dht_dev;
static int16_t temperature, humidity;

void read_dht(void) {
    while (1) {
        if (dht_read(&dht_dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   (int)temperature / 10, (int)temperature % 10,
                   (int)humidity / 10, (int)humidity % 10);

            if (temperature / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            puts("Failed to read from DHT sensor");
        }

        xtimer_sleep(SLEEP_INTERVAL);
    }
}

int main(void) {
    // Initialize GPIO for LED
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    // Initialize DHT11 sensor
    dht_params_t dht_params = {
        .pin = DHT_PIN,
        .type = DHT11
    };
    dht_init(&dht_dev, &dht_params);

    // Start the DHT reading thread
    thread_create(
        thread_stack(NULL, 512),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        read_dht,
        NULL,
        "dht_reader"
    );

    return 0;
}
