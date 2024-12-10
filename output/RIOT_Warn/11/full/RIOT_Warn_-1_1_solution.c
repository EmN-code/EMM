#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include "dht.h"
#include "dht_params.h"

#define DHT_SENSOR_PIN GPIO_PIN(PORT_A, 0)
#define LED_PIN GPIO_PIN(PORT_B, 0)
#define TEMP_THRESHOLD 30  // Temperature Threshold in degree Celsius

int main(void) {
    // Initialize DHT11 sensor
    dht_t dev;
    dht_init(&dev, &dht_params[0]);

    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    int16_t temp, hum;
    while (1) {
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Error reading from DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Warning: Temperature threshold exceeded! LED is ON.\n");
            } else {
                gpio_clear(LED_PIN);
            }
        }
        xtimer_sleep(5);  // sleep for 5 seconds
    }
    return 0;
}