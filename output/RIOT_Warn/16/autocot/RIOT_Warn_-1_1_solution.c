#include <stdio.h> 
#include "dht.h"  
#include "xtimer.h"
#include "periph/gpio.h"

#define DHT11_PIN GPIO_PIN(PORT_A, 0) // Pin where the DHT11 is connected
#define LED_PIN GPIO_PIN(PORT_B, 0)  // Pin where the LED is connected
#define TEMP_THRESHOLD 25  // Temperature threshold for the LED warning
#define DHT_READ_INTERVAL (5U * US_PER_SEC)  // Read interval in microseconds

int main(void) {
    dht_t dev;
    int16_t temp, hum;
    dht_params_t params = {
        .pin = DHT11_PIN,
        .type = DHT11,
    };

    // Initialize the DHT sensor
    if (dht_init(&dev, &params) != 0) {
        printf("DHT initialization failed\n");
        return 1;
    }

    // Initialize LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {  
        if (dht_read(&dev, &temp, &hum) != 0) {
            printf("DHT read failed\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                   temp / 10, abs(temp % 10), hum / 10, abs(hum % 10));

            // Check if the temperature exceeds the threshold
            if (temp / 10 > TEMP_THRESHOLD) {
                // Turn on the LED
                gpio_set(LED_PIN);
            } else {
                // Turn off the LED
                gpio_clear(LED_PIN);
            }
        }

        // Wait for the next reading interval
        xtimer_sleep(DHT_READ_INTERVAL / US_PER_SEC);
    }

    return 0;
}