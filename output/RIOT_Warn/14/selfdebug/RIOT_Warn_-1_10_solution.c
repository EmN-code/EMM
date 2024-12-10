#include "xtimer.h"
#include "thread.h"
#include "stdio.h"
#include "dht.h"
#include "periph/gpio.h"

#define DHT_PIN GPIO_PIN(PORT_A, 22)
#define DHT_TYPE DHT11
#define LED_PIN GPIO_PIN(PORT_B, 15)
#define TEMP_THRESHOLD 25
#define SLEEP_INTERVAL (5U * US_PER_SEC)

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    dht_t dev;

    if (dht_init(&dev, DHT_TYPE, DHT_PIN) != DHT_OK) {
        printf("DHT initialization failed\n");
        return 1;
    }

    while (1) {
        int16_t temp, hum;
        if (dht_measure(&dev, &temp, &hum) != DHT_OK) {
            printf("DHT measurement failed\n");
        } else {
            printf("Temperature: %d, Humidity: %d\n", temp, hum);
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        xtimer_sleep(SLEEP_INTERVAL);
    }
    return 0;
}