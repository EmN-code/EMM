#include <xtimer.h>
#include <periph/gpio.h>
#include "dht.h"

#define DHT_PIN         GPIO_PIN(PORT_A, 1)  // Replace PORT_A with the actual port
#define LED_PIN         GPIO_PIN(PORT_X, 2)  // Replace PORT_X with the actual port
#define TEMP_THRESHOLD  30
#define INTERVAL        (5U * US_PER_SEC)    // 5 seconds

static dht_t dev;

int main(void) {
    int16_t temp, hum;

    // Initialize DHT sensor
    if (dht_init(&dev, DHT11, DHT_PIN) != 0) {
        printf("Error initializing DHT11\n");
        return 1;
    }

    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Error reading DHT11\n");
        } else {
            printf("Temperature: %d, Humidity: %d\n", temp, hum);
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        xtimer_sleep(INTERVAL);
    }
    return 0;
}