
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "xtimer.h"
#include "dht.h"

#define LED_PIN             GPIO_PIN(1, 13)  // GPIO13 on ESP32
#define DHT11_PIN           GPIO_PIN(1, 4)   // GPIO4 on ESP32
#define TEMP_THRESHOLD      25              // Temperature threshold in Celsius
#define INTERVAL_MS         5000000         // 5 seconds in microseconds

static void read_dht11(dht_t *dev, int16_t *temp, int16_t *hum) {
    dht_read(dev, temp, hum);
}

int main(void) {
    dht_t dht11;
    int16_t temperature, humidity;

    // Initialize the DHT11 sensor
    if (dht_init(&dht11, DHT11_PIN) == DHT_OK) {
        printf("DHT11 sensor initialized successfully\n");
    } else {
        printf("Failed to initialize DHT11 sensor\n");
        return 1;
    }

    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_set(LED_PIN);  // Turn off the LED initially

    while (1) {
        // Read temperature and humidity from DHT11
        read_dht11(&dht11, &temperature, &humidity);

        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
               temperature / 10, temperature % 10,
               humidity / 10, humidity % 10);

        // Check if temperature exceeds the threshold
        if (temperature / 10 > TEMP_THRESHOLD) {
            gpio_clear(LED_PIN);  // Turn on the LED
        } else {
            gpio_set(LED_PIN);    // Turn off the LED
        }

        // Wait for 5 seconds
        xtimer_usleep(INTERVAL_MS);
    }

    return 0;
}
