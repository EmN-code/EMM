#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <dht.h>
#include <periph/gpio.h>

#define DHT_PIN GPIO_PIN(0, 4)  // Adjust based on your actual wiring
#define LED_PIN GPIO_PIN(0, 2)  // Adjust based on your actual wiring
#define TEMP_THRESHOLD 30        // Temperature threshold in Celsius
#define READ_INTERVAL (5U * US_PER_SEC) // 5 seconds interval

static dht_t dev;

int main(void) {
    // Initialize DHT sensor
    if (dht_init(&dev, DHT_MODEL_DHT11) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    // Initialize LED pin
    if (gpio_init(LED_PIN, GPIO_OUT) != 0) {
        printf("Error initializing LED pin\n");
        return 1;
    }

    printf("Starting DHT11 Temperature Monitor\n");

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Failed to read from DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, abs(temp % 10), hum / 10, abs(hum % 10));

            // Check if the temperature exceeds the threshold
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Temperature exceeds %d°C! LED ON\n", TEMP_THRESHOLD);
            } else {
                gpio_clear(LED_PIN);
                printf("Temperature is below %d°C. LED OFF\n", TEMP_THRESHOLD);
            }
        }

        // Wait for the next read interval
        xtimer_sleep(READ_INTERVAL);
    }

    return 0;
}