#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(0, 2)

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    /* Initialize the GPIO pin for DHT11 */
    if (dht_init(&dev, DHT_PIN, DHT11) != 0) {
        puts("Error initializing DHT11");
        return 1;
    }

    while (1) {
        /* Read temperature and humidity */
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            puts("Failed to read from DHT11 sensor");
        }

        /* Wait for 5 seconds */
        xtimer_sleep(5);
    }

    return 0;
}