
#include "periph/gpio.h"
#include "periph/dht.h"
#include "xtimer.h"

#define DHT_PIN             GPIO_PIN(0, 12)  // GPIO12 on ESP32
#define LED_PIN             GPIO_PIN(0, 2)   // GPIO2 on ESP32
#define TEMP_THRESHOLD      30               // Temperature threshold in Celsius
#define INTERVAL            5                // Interval in seconds

void read_dht11(dht_t *dev, int16_t *temp, int16_t *hum) {
    dht_read(dev, temp, hum);
}

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;

    // Initialize DHT11 sensor
    if (dht_init(&dev, DHT_PIN) == DHT_OK) {
        printf("DHT11 sensor initialized successfully\n");
    } else {
        printf("Failed to initialize DHT11 sensor\n");
        return 1;
    }

    // Initialize LED
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);  // Ensure LED is off initially

    while (1) {
        // Read temperature and humidity
        read_dht11(&dev, &temperature, &humidity);

        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
               temperature / 10, temperature % 10,
               humidity / 10, humidity % 10);

        // Check if temperature exceeds the threshold
        if (temperature / 10 >= TEMP_THRESHOLD) {
            gpio_set(LED_PIN);  // Turn on LED
        } else {
            gpio_clear(LED_PIN);  // Turn off LED
        }

        // Wait for 5 seconds
        xtimer_sleep(INTERVAL);
    }

    return 0;
}
