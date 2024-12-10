#include <xtimer.h>
#include <timex.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN         GPIO_PIN(PORT_A, 5)    // Example pin configuration
#define LED_PIN         GPIO_PIN(PORT_B, 10)   // Example pin configuration
#define TEMP_THRESHOLD  30                     // Example temp threshold in degree Celsius
#define INTERVAL        (5U * US_PER_SEC)      // Interval for reading every 5 seconds

dht_t dev;

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT11;
    dht_init(&dev, &params);

    int16_t temp, hum;

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);

            if ((temp / 10) > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            puts("Failed to read DHT sensor\n");
        }

        xtimer_sleep(INTERVAL / US_PER_SEC);
    }

    return 0;
}