#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <periph/gpio.h>
#include <periph/i2c.h>

#define DHT11_PORT_A GPIO_PIN(PORT_A, 0)
#define LED_PORT_B GPIO_PIN(PORT_B, 0)
#define TEMP_THRESHOLD 30
#define READ_INTERVAL (5U * US_PER_SEC)

dht_t dht11;

int main(void) {
    // Initialize DHT11 sensor
    if (dht_init(&dht11, &DHT11_PORT_A) != DHT_OK) {
        printf("Error: Failed to initialize DHT11 sensor\n");
        return 1;
    }

    // Initialize LED GPIO
    if (gpio_init(LED_PORT_B, GPIO_OUT) < 0) {
        printf("Error: Unable to initialize GPIO for LED\n");
        return 1;
    }

    int16_t temperature, humidity;

    while (1) {
        // Read temperature and humidity from DHT11
        if (dht_read(&dht11, &temperature, &humidity) != DHT_OK) {
            printf("Error: Failed to read from DHT11 sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                   temperature / 10, temperature % 10, 
                   humidity / 10, humidity % 10);

            // Check if temperature exceeds threshold
            if (temperature / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PORT_B);
                printf("Warning: Temperature threshold exceeded!\n");
            } else {
                gpio_clear(LED_PORT_B);
            }
        }
        // Wait for 5 seconds before next read
        xtimer_sleep(5);
    }
    return 0;
}
