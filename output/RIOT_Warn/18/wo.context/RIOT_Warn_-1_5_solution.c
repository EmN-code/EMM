#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 4)  // Example pin, it should be set according to board specs
#define LED_PIN GPIO_PIN(0, 5)  // Example pin for the LED
#define TEMP_THRESHOLD 30       // Temperature threshold in Celsius
#define DHT_TYPE DHT11          // Sensor type

static dht_t dev;

int main(void)
{
    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_PIN, DHT_TYPE) != DHT_OK) {
        puts("Failed to initialize DHT sensor\n");
        return 1;
    }

    // Initializing the LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        int16_t temp, hum;

        // Read values from the DHT sensor
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            puts("DHT sensor read failed\n");
        } else {
            printf("Temperature: %d°C, Humidity: %d%%\n", temp, hum);
            
            // Check if temperature exceeds threshold
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                puts("Warning: High temperature! LED On\n");
            } else {
                gpio_clear(LED_PIN);
                puts("Temperature normal. LED Off\n");
            }
        }

        // Wait 5 seconds
        xtimer_sleep(5);
    }
    return 0;
}