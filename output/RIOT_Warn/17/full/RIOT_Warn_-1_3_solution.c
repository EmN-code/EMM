#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(PORT_A, 0)
#define LED_PIN GPIO_PIN(PORT_B, 0)
#define TEMP_THRESHOLD 30  // Example temperature threshold

static dht_t dev;

int main(void) {
    // Initialize GPIO for the LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize DHT11 sensor
    if (dht_init(&dev, DHT_PIN) != DHT_OK) {
        printf("Failed to initialize DHT11\n");
        return -1;
    }

    int16_t temp, hum;

    // Continuously read data and check temperature
    while (1) {
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Error reading DHT11\n");
        }
        else {
            printf("Temperature: %d, Humidity: %d\n", temp, hum);
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            }
            else {
                gpio_clear(LED_PIN);
            }
        }
        xtimer_sleep(5);
    }

    return 0;
}