#include "xtimer.h"
#include "thread.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 0)  // Example pin, adjust as needed
#define LED_PIN GPIO_PIN(PORT_A, 1)  // Example pin, adjust as needed
#define THRESHOLD_TEMP 30            // Temperature threshold
#define SLEEP_INTERVAL (5U * 1000000U)   // 5 seconds in microseconds

int main(void) {
    dht_t dev;
    int16_t temp, hum;
    int res;
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    // Initialize DHT sensor
    res = dht_init(&dev, &DHT_PIN, DHT11);
    if (res != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    while (1) {
        res = dht_read(&dev, &temp, &hum);
        if (res == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            if (temp / 10 > THRESHOLD_TEMP) {
                printf("Temperature above threshold! Turning on LED.\n");
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }

        xtimer_sleep(SLEEP_INTERVAL / US_PER_MS);
    }
    return 0;
}
