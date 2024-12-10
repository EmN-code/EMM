#include "mma8x5x_regs.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "dht_params.h"

#include "dht.h"

#define DHT_PIN GPIO_PIN(x,y)  // Set the correct GPIO pin for DHT
#define LED_PIN GPIO_PIN(x,y)  // Set the correct GPIO pin for LED
#define TEMP_THRESHOLD 30  // Set your temperature threshold

int main(void) {
    // Initialize the DHT sensor
    dht_t dev;
    if (dht_init(&dev, &dht_params[0]) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    // Configure LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    int16_t temp, hum;

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%RH\n", temp / 10, temp % 10, hum / 10, hum % 10);

            // Check if temperature exceeds the threshold
            if (temp > TEMP_THRESHOLD * 10) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_SEC, 5);
    }
    return 0;
}
