#include <stdio.h>
#include "xtimer.h"
#include "timex.h"
#include "dht.h"
#include "periph/gpio.h"

#define DHT_PIN GPIO_PIN(PORT_B, 7)
#define LED_PIN GPIO_PIN(PORT_A, 5)
#define TEMP_THRESHOLD 30 // Temperature threshold in Celsius

int main(void) {
    // Initialize the DHT sensor
    dht_t dev;
    if (dht_init(&dev, DHT_PIN) != DHT_OK) {
        printf("DHT initialization failed\n");
        return 1;
    }
    printf("DHT initialized at pin %d\n", DHT_PIN);

    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    int16_t temp, hum;
    while (1) {
        // Read from DHT sensor
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
            // Check if temperature exceeds the threshold
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Warning: Temperature exceeds threshold!\n");
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        // Wait for 5 seconds
        xtimer_sleep(5);
    }
    return 0;
}