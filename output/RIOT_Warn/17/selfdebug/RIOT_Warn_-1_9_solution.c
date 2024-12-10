#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(PORT_B, 2)
#define LED_PIN GPIO_PIN(PORT_A, 5)
#define TEMP_THRESHOLD 25 // Temperature threshold in Celsius

int main(void) {
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT11;

    gpio_init(LED_PIN, GPIO_OUT);

    dht_t dev;
    if (dht_init(&dev, &params) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C\n", temp / 10, temp % 10);
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Temperature exceeds threshold! LED on.\n");
            } else {
                gpio_clear(LED_PIN);
                printf("Temperature is normal. LED off.\n");
            }
        } else {
            printf("Failed to read data from DHT sensor\n");
        }

        xtimer_sleep(5);
    }

    return 0;
}