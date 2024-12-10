#include "xtimer.h"  // For xtimer_sleep
#include "periph/gpio.h"  // For GPIO operations
#include "dht.h"  // For DHT sensor operations

#define DHT_PIN GPIO_PIN(PORT_X, 0)  // Assume PORT_X and logical pin 0 for DHT
#define LED_PIN GPIO_PIN(PORT_Y, 1)  // Assume PORT_Y and logical pin 1 for LED
#define TEMP_THRESHOLD 30  // Temperature threshold for LED warning

void read_dht_temperature(void) {
    dht_t dev;
    if (dht_init(&dev, DHT_PIN) != 0) {
        printf("Failed to initialize DHT sensor\n");
        return;
    }

    int16_t temperature, humidity;
    if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
        printf("Temperature: %0.1f°C, Humidity: %0.1f%%\n", temperature / 10.0, humidity / 10.0);
        if (temperature / 10 >= TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
            printf("Warning: Temperature exceeds threshold!\n");
        } else {
            gpio_clear(LED_PIN);
        }
    } else {
        printf("Failed to read DHT sensor\n");
    }
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    printf("Starting DHT11 sensor monitoring\n");

    while (1) {
        read_dht_temperature();
        xtimer_sleep(5);
    }

    return 0;
}