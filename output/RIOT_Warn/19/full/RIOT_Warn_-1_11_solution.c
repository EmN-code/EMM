#include <xtimer.h>
#include <periph/gpio.h>
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 0) /* Change to the proper GPIO pin */
#define LED_PIN GPIO_PIN(PORT_B, 1) /* Change to the proper GPIO pin */
#define TEMP_THRESHOLD 25 /* Temperature threshold in Celsius */

int main(void)
{
    gpio_init(LED_PIN, GPIO_OUT);

    dht_t dev;
    if (dht_init(&dev, DHT_PIN) != DHT_OK) {
        printf("Initialization failed\n");
        return -1;
    }

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read sensor\n");
        }
        xtimer_sleep(5);
    }

    return 0;
}