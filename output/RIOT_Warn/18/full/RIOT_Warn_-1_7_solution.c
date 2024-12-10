#include <xtimer.h>
#include <stdio.h>
#include <dht.h>
#include <periph/gpio.h>

#define DHT_PIN GPIO_PIN(PORT_A, 5)
#define LED_PIN GPIO_PIN(PORT_A, 15)
#define TEMP_THRESHOLD 30  // Temperature threshold in degree Celsius

int main(void)
{
    dht_t dev;
    if (dht_init(&dev, DHT_PIN, DHT11) != DHT_OK) {
        puts("Failed to initialize DHT11 sensor");
        return 1;
    }
    gpio_init(LED_PIN, GPIO_OUT);
    int16_t temp, hum;

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C
", temp / 10, temp % 10);
            printf("Humidity: %d.%d%%
", hum / 10, hum % 10);

            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            puts("Failed to read from DHT11 sensor");
        }

        xtimer_sleep(5);
    }

    return 0;
}