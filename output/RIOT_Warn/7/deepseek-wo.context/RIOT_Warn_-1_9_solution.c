
#include "periph/gpio.h"
#include "periph/timer.h"
#include "dht.h"
#include "xtimer.h"

#define DHT_PIN             GPIO_PIN(0, 4)  // GPIO4 on ESP32
#define LED_PIN             GPIO_PIN(0, 2)  // GPIO2 on ESP32
#define TEMP_THRESHOLD      25              // Temperature threshold in Celsius
#define INTERVAL_MS         5000000         // 5 seconds in microseconds

static void read_dht(void) {
    dht_t dev;
    int16_t temp, hum;

    if (dht_init(&dev, DHT_PIN) != DHT_OK) {
        printf("DHT11 initialization failed\n");
        return;
    }

    if (dht_read(&dev, &temp, &hum) != DHT_OK) {
        printf("DHT11 read failed\n");
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
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    while (1) {
        read_dht();
        xtimer_usleep(INTERVAL_MS);
    }

    return 0;
}
