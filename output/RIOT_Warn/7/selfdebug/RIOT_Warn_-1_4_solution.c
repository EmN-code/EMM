#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <periph/gpio.h>
#include <dht.h>

#define LED_PIN GPIO_PIN(PORT_B, 0)
#define DHT11_PIN GPIO_PIN(PORT_A, 8)
#define TEMP_THRESHOLD 30

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;

    /* Initialize the DHT11 sensor */
    if (dht_init(&dev, DHT11_PIN) != 0) {
        printf("Error: Unable to initialize DHT11 sensor\n");
        return 1;
    }

    /* Initialize LED pin */
    if (gpio_init(LED_PIN, GPIO_OUT) != 0) {
        printf("Error: Unable to initialize LED pin\n");
        return 1;
    }

    while (1) {
        /* Read values from DHT11 sensor */
        if (dht_read(&dev, &temperature, &humidity) != 0) {
            printf("Error: Unable to read from DHT11 sensor\n");
        } else {
            printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);

            /* Turn on LED if temperature exceeds threshold */
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Warning: Temperature exceeded threshold!\n");
            } else {
                gpio_clear(LED_PIN);
            }
        }

        /* Wait for 5 seconds */
        xtimer_sleep(5);
    }

    return 0;
}