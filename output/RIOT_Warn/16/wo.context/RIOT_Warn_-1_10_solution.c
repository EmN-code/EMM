#include <xtimer.h>
#include <stdio.h>
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN         GPIO_PIN(PORT_A, 0)  // Adjust pin values as per actual wiring
#define LED_PIN         GPIO_PIN(PORT_B, 0)  // Adjust pin values as per actual wiring
#define TEMP_THRESHOLD  30  // Temperature threshold in degrees Celsius
#define INTERVAL        (5U * US_PER_SEC)  // 5 seconds interval

int main(void) {
    // Initialize DHT sensor
    dht_t dev;
    if (dht_init(&dev, DHT_PIN, DHT11) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN); // Ensure the LED is off initially

    int16_t temp, hum;
    while (1) {
        // Read temperature and humidity from DHT11
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                    temp / 10, temp % 10, hum / 10, hum % 10);
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);   // Turn on LED
            } else {
                gpio_clear(LED_PIN); // Turn off LED
            }
        } else {
            printf("Failed to read data from DHT sensor\n");
        }

        // Sleep for the defined interval
        xtimer_sleep(INTERVAL / US_PER_SEC);
    }
    return 0;
}