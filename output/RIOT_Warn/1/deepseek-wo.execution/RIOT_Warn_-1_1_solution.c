
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

void read_dht11(void) {
    while (1) {
        if (dht_read(&dht_dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);

            if (temperature / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);  // Turn on LED
            } else {
                gpio_clear(LED_PIN);  // Turn off LED
            }
        } else {
            puts("Failed to read from DHT11 sensor");
        }

        xtimer_sleep(SLEEP_INTERVAL);  // Sleep for 5 seconds
    }
}

int main(void) {
    // Initialize DHT11 sensor
    dht_params_t dht_params = {
        .pin = DHT_PIN,
        .type = DHT11
    };
    dht_init(&dht_dev, &dht_params);

    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Start the DHT11 reading thread
    thread_create(
        thread_stack(read_dht11),
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        0,
        read_dht11,
        NULL,
        "dht11_reader"
    );

    return 0;
}
