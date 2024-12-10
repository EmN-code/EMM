#include <xtimer.h>
#include <stdio.h>
#include <dht.h>
#include <dht_params.h>
#include <periph/gpio.h>

#define DHT_PIN GPIO_PIN(PORT_A, 0)
#define LED_PIN GPIO_PIN(PORT_B, 1)
#define TEMP_THRESHOLD 30 // Threshold temperature in Celsius

void init_sensors(void) {
    dht_t dev;
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT11;

    // Initialize DHT11 sensor
    if(dht_init(&dev, &params) != 0) {
        printf("Error initializing DHT11 sensor\n");
        return;
    }

    // Initialize LED pin
    if(gpio_init(LED_PIN, GPIO_OUT) != 0) {
        printf("Error initializing LED pin\n");
        return;
    }

    int16_t temp, hum;

    while (1) {
        int err = dht_read(&dev, &temp, &hum);

        if(err != DHT_OK) {
            printf("Error reading DHT11 sensor\n");
            xtimer_sleep(1);
            continue;
        }

        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);

        if (temp / 10 > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
            printf("Warning: Temperature threshold exceeded!\n");
        } else {
            gpio_clear(LED_PIN);
        }

        xtimer_sleep(5);
    }
}

int main(void) {
    init_sensors();
    return 0;
}