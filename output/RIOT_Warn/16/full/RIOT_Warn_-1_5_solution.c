#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT11_PWR_PIN GPIO_PIN(PORT_X, 0)
#define DHT11_DATA_PIN GPIO_PIN(PORT_X, 1)
#define LED_PIN GPIO_PIN(PORT_Y, 0)
#define TEMP_THRESHOLD 30  // Temperature threshold in degrees Celsius
#define DHT_TIMEOUT 100

static dht_t dev;

int main(void) {
    // Initialize the DHT11 sensor
    if (dht_init(&dev, DHT11_DATA_PIN) != DHT_OK) {
        return 1;  // Return with error if sensor initialization fails
    }

    // Configure the LED_PIN as output
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Turn on power to DHT11
    gpio_init(DHT11_PWR_PIN, GPIO_OUT);
    gpio_set(DHT11_PWR_PIN);
    
    while (1) {
        int16_t temp, hum;

        // Reading the DHT11 sensor
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %dC, Humidity: %d%%\n", temp, hum);
            
            if (temp > TEMP_THRESHOLD) {
                // Turn on LED if temperature exceeds the threshold
                gpio_set(LED_PIN);
                printf("Temperature warning: %dC\n", temp);
            } else {
                // Turn off LED if temperature is below the threshold
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read DHT11 sensor, trying again\n");
        }

        xtimer_sleep(5);
    }

    return 0;
}