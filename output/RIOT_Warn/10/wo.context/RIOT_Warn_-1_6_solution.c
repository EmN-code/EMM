#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 8)  // Update this line for correct usage
#define LED_PIN GPIO_PIN(PORT_B, 0)  // Example LED pin definition
#define TEMP_THRESHOLD 30

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    dht_t dev;
    dht_params_t params = { .pin = DHT_PIN, .type = DHT11 };
    dht_init(&dev, &params);

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from sensor\n");
        }

        xtimer_sleep(5);
    }
    return 0;
}