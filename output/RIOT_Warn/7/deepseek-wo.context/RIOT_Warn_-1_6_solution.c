
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN             GPIO_PIN(0, 12)  // GPIO12 on ESP32
#define LED_PIN             GPIO_PIN(0, 2)   // GPIO2 on ESP32
#define TEMP_THRESHOLD      30               // Temperature threshold in Celsius
#define INTERVAL_MS         5000000          // 5 seconds in microseconds

static void read_dht11(void) {
    dht_t dev;
    int16_t temp, hum;

    if (dht_init(&dev, DHT_PIN) != DHT_OK) {
        puts("DHT11 initialization failed");
        return;
    }

    if (dht_read(&dev, &temp, &hum) != DHT_OK) {
        puts("DHT11 read failed");
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
    if (gpio_init(LED_PIN, GPIO_OUT) < 0) {
        puts("Failed to initialize LED GPIO");
        return 1;
    }

    while (1) {
        read_dht11();
        xtimer_usleep(INTERVAL_MS);
    }

    return 0;
}
