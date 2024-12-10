#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(PORT_A, 0) // Adjusted with a valid port identifier
#define LED_PIN GPIO_PIN(PORT_A, 1) // Adjusted with a valid port identifier
#define TEMP_THRESHOLD 30 // Temperature threshold in Celsius

void check_temperature(void) {
    dht_t dev;
    int16_t temp, hum;
    dht_init(&dev, DHT_PIN, DHT11);
    while (1) {
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Failed to read from DHT11 sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10,
                   hum / 10, hum % 10);
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        xtimer_sleep(5);
    }
}

int main(void) {
    // Initialize GPIO pins
    gpio_init(LED_PIN, GPIO_OUT);
    check_temperature();
    return 0;
}