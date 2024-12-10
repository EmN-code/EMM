#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN         GPIO_PIN(PORT_A, 5)   // Example GPIO_PIN which might correspond to actual ESP32 pin
#define THRESHOLD       30
#define INTERVAL_SEC    5
#define LED_PIN         GPIO_PIN(PORT_B, 0)   // Another example hard-coded GPIO_PIN

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    // Initialize GPIO for LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the sensor. Note: Correcting to appropriate initialization parameters for ESP32.
    if (dht_init(&dev, DHT11, DHT_PIN) != DHT_OK) {
        printf("Failed to initialize DHT
");
        return 1;
    }

    while (1) {
        if (dht_measure(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d°C, Humidity: %d%%\n", temp / 10, hum / 10);

            if ((temp / 10) > THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Warning: Temperature exceeding threshold!\n");
            }
            else {
                gpio_clear(LED_PIN);
            }
        }
        else {
            printf("Failed to read from sensor
");
        }

        xtimer_sleep(INTERVAL_SEC);
    }
    return 0;
}