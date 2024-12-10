#include "mma8x5x_params.h"
#include "dht_params.h"
#include "dht.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "xtimer.h"

#define TEMP_THRESHOLD 30  // Threshold temperature for LED warning in degree Celsius
#define DHT_PIN GPIO_PIN(0)  // Example pin for DHT11 sensor
#define LED_PIN GPIO_PIN(2)  // Example pin for LED

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;
    int rc;
 
    // Initialize DHT sensor
    while ((rc = dht_init(&dev, &dht_params[0])) != 0) {
        printf("Failed to initialize DHT sensor\n");
        xtimer_sleep(1);
    }

    // Configure LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temperature, &humidity) != 0) {
            printf("Failed to read from DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);
            
            // Check temperature and control LED
            if (temperature > TEMP_THRESHOLD * 10) {
                gpio_set(LED_PIN);
                printf("Temperature exceeded threshold! LED ON.\n");
            } else {
                gpio_clear(LED_PIN);
                printf("Temperature below threshold. LED OFF.\n");
            }
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }
    return 0;
}