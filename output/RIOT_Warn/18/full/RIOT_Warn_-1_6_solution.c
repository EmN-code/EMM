#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(PORT_B, 6)  // Assume the DHT sensor is connected to GPIO pin 6 of port B
#define LED_PIN GPIO_PIN(PORT_B, 7)  // Assume the LED is connected to GPIO pin 7 of port B
#define TEMP_THRESHOLD 30  // Temperature threshold to turn on the LED

int main(void) {
    dht_t dev;
    gpio_init(LED_PIN, GPIO_OUT);
    xtimer_ticks32_t last = xtimer_now();

    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_TYPE_DHT11, DHT_PIN) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            if (temp > TEMP_THRESHOLD * 10) {
                gpio_set(LED_PIN);
                printf("LED turned ON\n");
            } else {
                gpio_clear(LED_PIN);
                printf("LED turned OFF\n");
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }

        // Wait for 5 seconds
        xtimer_periodic_wake(&last, 5 * US_PER_SEC);
    }
    return 0;
}