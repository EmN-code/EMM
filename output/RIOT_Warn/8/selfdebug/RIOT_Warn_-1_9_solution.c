#include <xtimer.h>
#include <stdio.h>
#include <dht.h>
#include <periph/gpio.h>

#define LED_PIN     GPIO_PIN(PORT_A, 5) // Define a suitable GPIO port and pin for LED
#define DHT_PIN     GPIO_PIN(PORT_A, 6) // Define a suitable GPIO port and pin for DHT11
#define TEMP_THRESHOLD 30 // Temperature threshold in Celsius

dht_t dev;

void check_temperature(void) {
    int16_t temp, hum;
    if (dht_read(&dev, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        if (temp >= TEMP_THRESHOLD * 10) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }
    } else {
        printf("Failed to read from DHT11\n");
    }
}

int main(void) {
    // Initialize the GPIO for the LED
    gpio_init(LED_PIN, GPIO_OUT);
    // Initialize the DHT11
    if (dht_init(&dev, DHT_PIN, DHT_TYPE_DHT11) == DHT_OK) {
        puts("DHT11 sensor initialized successfully");
    } else {
        puts("Failed to initialize DHT11 sensor");
        return 1;
    }

    // Main loop to check temperature every 5 seconds
    while (1) {
        check_temperature();
        xtimer_sleep(5);
    }
    return 0;
}