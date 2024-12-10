#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 0)
#define LED_PIN GPIO_PIN(PORT_B, 1)
#define TEMP_THRESHOLD 30  // Temperature threshold for the LED to turn on

static dht_t dht_dev;

int main(void) {
    // Initialize DHT11
    if (dht_init(&dht_dev, DHT11, DHT_PIN) != 0) {
        printf("Error initializing DHT11 sensor\n");
        return 1;
    }
    
    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    int16_t temp, hum;
    while (1) {
        // Read DHT11 sensor
        if (dht_read(&dht_dev, &temp, &hum) != DHT_OK) {
            printf("Failed to read DHT11 sensor\n");
        } else {
            printf("Temperature: %d°C, Humidity: %d%%\n", temp, hum);
            // Check temperature and turn on LED if threshold is exceeded
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        // Wait for 5 seconds before next reading
        xtimer_sleep(5);
    }
    return 0;
}