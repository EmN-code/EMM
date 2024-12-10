#include "xtimer.h"
#include "saul_reg.h"
#include "dht.h"
#include "dht_params.h"
#include <stdio.h>

#define DHT_TYPE DHT11
#define DHT_PIN  GPIO_PIN(PORT_X, X) // Replace PORT_X and X with the actual port and pin
#define TEMP_THRESHOLD 30  // temperature threshold to trigger LED warning
#define SLEEP_INTERVAL (5U * US_PER_SEC)  // 5 seconds
#define LED_PIN GPIO_PIN(PORT_X, X) // Replace PORT_X and X with the LED pin

int main(void) {
    // Initialize the DHT sensor
    dht_t dev;
    dht_params_t params = {
        .type = DHT_TYPE,
        .pin = DHT_PIN
    };
    if (dht_init(&dev, &params) != DHT_OK) {
        printf("Initialization failed\n");
        return 1;
    }

    // Configure the LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Failed to read data\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%% \n",
                   temp / 10, temp % 10, hum / 10, hum % 10);

            // Check temperature and control LED
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Temperature exceeded threshold! LED is ON.\n");
            } else {
                gpio_clear(LED_PIN);
                printf("Temperature is normal. LED is OFF.\n");
            }
        }
        // Sleep for defined interval
        xtimer_sleep(SLEEP_INTERVAL / US_PER_SEC);
    }
    return 0;
}