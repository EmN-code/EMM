#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_TYPE     DHT11
#define GPIO_PIN     GPIO_PIN(PORT_A, 5)  // Assuming PORT A and pin 5 for illustration
#define LED_PIN      GPIO_PIN(PORT_B, 0)  // Assuming PORT B and pin 0 for LED
#define TEMP_THRESHOLD  30  // Set the temperature threshold

static dht_t dev;

int main(void) {
    int16_t temp, hum;
    gpio_init(LED_PIN, GPIO_OUT);
    if (dht_init(&dev, &DHT_TYPE, GPIO_PIN) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, abs(temp % 10), hum / 10, abs(hum % 10));
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read DHT sensor\n");
        }
        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    return 0;
}