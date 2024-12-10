
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN             GPIO_PIN(1, 14)  // GPIO14 on ESP32
#define LED_PIN             GPIO_PIN(1, 2)   // GPIO2 on ESP32
#define TEMP_THRESHOLD      30               // Temperature threshold in Celsius
#define INTERVAL_MS         5000000          // 5 seconds interval in microseconds

static void read_dht(void) {
    dht_t dev;
    int16_t temp, hum;

    if (dht_init(&dev, DHT_PIN, DHT_TYPE_DHT11) != DHT_OK) {
        printf("DHT11 initialization failed\n");
        return;
    }

    if (dht_read(&dev, &temp, &hum) != DHT_OK) {
        printf("Failed to read data from DHT11\n");
        return;
    }

    printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
           temp / 10, temp % 10, hum / 10, hum % 10);

    if (temp / 10 > TEMP_THRESHOLD) {
        gpio_set(LED_PIN);
    } else {
        gpio_clear(LED_PIN);
    }
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();

    if (gpio_init(LED_PIN, GPIO_OUT) < 0) {
        printf("Failed to initialize LED GPIO\n");
        return 1;
    }

    while (1) {
        read_dht();
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL_MS);
    }

    return 0;
}
