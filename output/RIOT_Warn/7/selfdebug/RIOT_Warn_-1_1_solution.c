#include "xtimer.h"
#include "thread.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_B, 5)
#define LED_PIN GPIO_PIN(PORT_B, 0)
#define TEMP_THRESHOLD 30
#define SLEEP_INTERVAL (5U * US_PER_SEC)

int main(void) {
    dht_t dev;
    int16_t temp, hum;
    gpio_init(LED_PIN, GPIO_OUT);
    dht_init(&dev, &(dht_params_t){.pin = DHT_PIN, .type = DHT11});

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from DHT11 sensor.\n");
        }
        xtimer_sleep(SLEEP_INTERVAL);
    }
    return 0;
}