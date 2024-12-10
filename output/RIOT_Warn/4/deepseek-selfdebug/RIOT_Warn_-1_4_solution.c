
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN             GPIO_PIN(0, 18)  // GPIO pin for DHT11
#define LED_PIN             GPIO_PIN(0, 21)  // GPIO pin for LED
#define TEMP_THRESHOLD      25               // Temperature threshold in Celsius
#define INTERVAL_MS         5000000          // Interval for reading DHT11 in microseconds

static void read_dht11(void) {
    dht_t dev;
    int16_t temp, hum;

    if (dht_init(&dev, DHT_PIN) != DHT_OK) {
        printf("DHT11 initialization failed\n");
        return;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Failed to read data from DHT11\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);

            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }

        xtimer_usleep(INTERVAL_MS);
    }
}

int main(void) {
    if (gpio_init(LED_PIN, GPIO_OUT) < 0) {
        printf("Failed to initialize LED GPIO\n");
        return 1;
    }

    read_dht11();

    return 0;
}
