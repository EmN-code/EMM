#include <stdio.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(0, 1) // Update to a correct GPIO pin for the DHT11
#define LED_PIN GPIO_PIN(0, 2) // Update to a correct GPIO pin for the LED
#define TEMP_THRESHOLD 30 // Define the temperature threshold

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    if (dht_init(&dev, DHT_TYPE_DHT11, DHT_PIN) != DHT_OK) {
        printf("DHT11 initialization failed\n");
        return 1;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            if (temp > TEMP_THRESHOLD * 10) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }

        xtimer_sleep(5); // sleep for 5 seconds
    }
    return 0;
}