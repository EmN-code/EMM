#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN         GPIO_PIN(PORT_A, 0) // Adjust to the correct port and pin
#define LED_PIN         GPIO_PIN(PORT_B, 1) // Adjust to the correct port and pin
#define TEMP_THRESHOLD  30                  // Temperature threshold in Celsius

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    dht_t dev;
    int16_t temp, hum;

    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_TYPE_DHT11, DHT_PIN) != DHT_OK) {
        printf("DHT initialization failed\n");
        return -1;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read DHT sensor\n");
        }

        xtimer_sleep(5);
    }

    return 0;
}