#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define LED_PIN     GPIO_PIN(PORT_A, 7)  // Define the LED pin
#define DHT_PIN     GPIO_PIN(PORT_B, 6)  // Define the DHT pin
#define TEMP_THRESH 30  // Define temperature threshold
#define DELAY       (5U * US_PER_SEC)  // 5 seconds delay

int main(void) {
    dht_t dev;
    int16_t temp, hum;
    int res;

    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_PIN, DHT11) != DHT_OK) {
        puts("Error initializing DHT sensor");
        return 1;
    }

    // Initialize the LED pin
    if (gpio_init(LED_PIN, GPIO_OUT) != 0) {
        puts("Error initializing LED pin");
        return 1;
    }

    // Main loop
    while (1) {
        // Read DHT sensor
        res = dht_read(&dev, &temp, &hum);
        if (res != DHT_OK) {
            printf("Error reading DHT sensor: %d\n", res);
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                    temp / 10, temp % 10, hum / 10, hum % 10);

            // Check temperature threshold
            if (temp >= TEMP_THRESH * 10) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }

        // Sleep for the specified delay
        xtimer_sleep(DELAY / US_PER_SEC);
    }

    return 0;
}