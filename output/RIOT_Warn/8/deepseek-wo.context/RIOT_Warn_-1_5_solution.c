
#include <stdio.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN         GPIO_PIN(0, 18)  // Replace with your actual DHT11 pin
#define LED_PIN         GPIO_PIN(0, 21)  // Replace with your actual LED pin
#define TEMP_THRESHOLD  30               // Replace with your desired temperature threshold
#define INTERVAL        5                // Interval in seconds

static void read_dht11(dht_t *dev, int16_t *temp, int16_t *hum) {
    dht_read(dev, temp, hum);
}

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    // Initialize DHT11 sensor
    if (dht_init(&dev, DHT_PIN) == DHT_OK) {
        printf("DHT11 sensor initialized successfully\n");
    } else {
        printf("Failed to initialize DHT11 sensor\n");
        return 1;
    }

    // Initialize LED
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_set(LED_PIN);  // Turn off LED initially

    while (1) {
        // Read temperature and humidity
        read_dht11(&dev, &temp, &hum);
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);

        // Check if temperature exceeds the threshold
        if (temp / 10 > TEMP_THRESHOLD) {
            gpio_clear(LED_PIN);  // Turn on LED
        } else {
            gpio_set(LED_PIN);    // Turn off LED
        }

        // Wait for the next reading
        xtimer_sleep(INTERVAL);
    }

    return 0;
}
