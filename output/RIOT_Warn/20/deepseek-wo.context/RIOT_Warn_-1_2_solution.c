
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN             GPIO_PIN(0, 18)  // GPIO pin for DHT11
#define LED_PIN             GPIO_PIN(0, 21)  // GPIO pin for LED
#define TEMP_THRESHOLD      25               // Temperature threshold in Celsius
#define INTERVAL_MS         5000000          // 5 seconds interval in microseconds

static dht_t dht_dev;

void read_dht11(void) {
    int16_t temp, hum;
    if (dht_read(&dht_dev, &temp, &hum) == DHT_OK) {
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
}

int main(void) {
    // Initialize the DHT11 sensor
    dht_params_t dht_params = {
        .pin = DHT_PIN,
        .type = DHT11,
        .in_mode = DHT_PARAM_PULL,  // Ensure this is defined in your DHT driver
    };
    dht_init(&dht_dev, &dht_params);

    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        read_dht11();
        xtimer_usleep(INTERVAL_MS);
    }

    return 0;
}
