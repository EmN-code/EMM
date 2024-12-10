#include "periph/gpio.h"
#include "dht.h"
#include "xtimer.h"

#define DHT_PIN GPIO_PIN(0, 5) // Assuming GPIO port 0, pin 5
#define LED_PIN GPIO_PIN(0, 6) // Assuming GPIO port 0, pin 6
#define TEMP_THRESHOLD 30     // Temperature threshold in Celsius
#define INTERVAL (5U * 1000000U) // 5 seconds

void dht11_read(void) {
    int16_t temperature, humidity;

    if (dht_read(DHT_PIN, &temperature, &humidity) == DHT_OK) {
        if (temperature > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }
    }
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_init(DHT_PIN, GPIO_IN);

    while (1) {
        dht11_read();
        xtimer_sleep(INTERVAL);
    }
    return 0;
}