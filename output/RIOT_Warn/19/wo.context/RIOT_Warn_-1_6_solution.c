
#include <stdio.h>
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 0)  // Define the pin where the DHT11 is connected.
#define LED_PIN GPIO_PIN(PORT_B, 0)  // Define the pin where the LED is connected.
#define TEMP_THRESHOLD 30  // Temperature threshold to trigger the LED.
#define INTERVAL (5U * US_PER_SEC)  // 5 seconds interval

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    // Initialize DHT sensor
    if (dht_init(&dev, DHT_PIN, DHT11) != DHT_OK) {
        printf("Error initializing DHT11\n");
        return 1;
    }

    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            if ((temp / 10) > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);  // Turn on the LED
            } else {
                gpio_clear(LED_PIN);  // Turn off the LED
            }
        } else {
            printf("Failed to read from sensor\n");
        }
        xtimer_sleep(INTERVAL);
    }

    return 0;
}
