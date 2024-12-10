#include <xtimer.h>
#include <stdio.h>
#include <dht.h>
#include <board_common.h>
#include "periph/adc.h"
#include "periph/gpio.h"

#define SENSOR_PIN GPIO_PIN(0, 4)  // Adjust pin number
#define LED_PIN GPIO_PIN(0, 5)     // LED connected to this pin
#define TEMPERATURE_THRESHOLD 30   // Temperature threshold in Celsius
#define DELAY_INTERVAL (5U * 1000000U)  // 5 seconds in microseconds

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    if (dht_init(&dev, SENSOR_PIN, DHT11) != DHT_OK) {
        printf("DHT sensor initialization failed\n");
        return -1;
    }

    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Failed to read from DHT sensor\n");
        } else {
            printf("Temperature: %d°C, Humidity: %d%%\n", temp, hum);

            if (temp > TEMPERATURE_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }

        xtimer_usleep(DELAY_INTERVAL);
    }

    // Cleanup and return
    return 0;
}