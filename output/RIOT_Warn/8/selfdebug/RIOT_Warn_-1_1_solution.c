#include <dht.h>
#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define INTERVAL_USEC (5U * US_PER_SEC) // 5 seconds
#define TEMP_THRESHOLD 30
#define LED_PIN GPIO_PIN(PORT_A, 5)

int main(void) {
    dht_t dev;
    int16_t temp, hum;
    dht_params_t params = {
        .pin = GPIO_PIN(PORT_A, 6),
        .type = DHT11
    };
    
    // Initialize the DHT sensor
    if (dht_init(&dev, &params) != 0) {
        printf("DHT initialization failed\n");
        return 1;
    }

    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        // Read temperature and humidity from the DHT11 sensor
        if (dht_read(&dev, &temp, &hum) != 0) {
            printf("Failed to read the DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            if (temp / 10 > TEMP_THRESHOLD) {
                // Turn on LED
                gpio_set(LED_PIN);
            } else {
                // Turn off LED
                gpio_clear(LED_PIN);
            }
        }

        // Wait for the next cycle
        xtimer_usleep(INTERVAL_USEC);
    }
    return 0;
}