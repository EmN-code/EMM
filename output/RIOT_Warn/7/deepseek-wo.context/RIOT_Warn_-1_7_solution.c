
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN             GPIO_PIN(0, 12)  // GPIO12 on ESP32
#define LED_PIN             GPIO_PIN(0, 2)   // GPIO2 on ESP32
#define TEMP_THRESHOLD      30               // Temperature threshold in Celsius
#define SLEEP_INTERVAL      (5 * 1000 * 1000) // 5 seconds in microseconds

static void read_dht11(dht_t *dev, int16_t *temp, int16_t *hum) {
    dht_read(dev, temp, hum);
}

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    // Initialize DHT11 sensor
    if (dht_init(&dev, DHT_TYPE_DHT11, DHT_PIN) != DHT_OK) {
        printf("DHT11 initialization failed\n");
        return 1;
    }

    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_set(LED_PIN);  // Turn off LED initially

    while (1) {
        // Read temperature and humidity
        read_dht11(&dev, &temp, &hum);

        // Print temperature and humidity
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
               temp / 10, temp % 10, hum / 10, hum % 10);

        // Check if temperature exceeds the threshold
        if (temp / 10 > TEMP_THRESHOLD) {
            gpio_clear(LED_PIN);  // Turn on LED
        } else {
            gpio_set(LED_PIN);    // Turn off LED
        }

        // Sleep for 5 seconds
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}
