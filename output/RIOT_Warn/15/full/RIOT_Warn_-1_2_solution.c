#include "string.h"
#include "event/thread.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 5)
#define LED_PIN GPIO_PIN(0, 15)
#define TEMP_THRESHOLD 30  // Temperature threshold in degree Celsius
#define SLEEP_INTERVAL_US 5000000  // 5 seconds

int main(void) {
    dht_t dev;
    int16_t temperature = 0, humidity = 0;

    if (dht_init(&dev, DHT_PIN) != DHT_OK) {
        printf("DHT initialization failed\n");
        return 1;
    }

    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
            printf("Failed to read from DHT sensor\n");
        } else {
            printf("Temperature: %d, Humidity: %d\n", temperature / 10, humidity / 10);

            if (temperature / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Temperature exceeded threshold. LED is ON.\n");
            } else {
                gpio_clear(LED_PIN);
                printf("Temperature within safe limits. LED is OFF.\n");
            }
        }
        xtimer_usleep(SLEEP_INTERVAL_US);
    }
    return 0;
}