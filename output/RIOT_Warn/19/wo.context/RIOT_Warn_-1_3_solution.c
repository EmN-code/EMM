#include <timex.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <periph/gpio.h>
#include <periph/i2c.h>

#include "dht.h"
#include "xtimer.h"

#define LED_PIN GPIO_PIN(PORT_A, 5)
#define DHT_PIN GPIO_PIN(PORT_B, 0)
#define TEMP_THRESHOLD 30
#define SLEEP_TIME (5U * US_PER_SEC)

int main(void) {
    // Initialize the DHT sensor
    if (dht_init(DHT_PIN) != 0) {
        printf("Error initializing DHT sensor\n");
        return -1;
    }

    // Set the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    int16_t temp, hum;

    while (1) {
        if (dht_read(DHT_PIN, &temp, &hum) != 0) {
            printf("Failed to read from DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);

            if (temp > TEMP_THRESHOLD * 10) { // Convert to deci-degrees
                gpio_set(LED_PIN);
                printf("Temperature exceeds threshold! LED is ON.\n");
            } else {
                gpio_clear(LED_PIN);
                printf("Temperature is below threshold. LED is OFF.\n");
            }
        }
        // Sleep for defined time
        xtimer_usleep(SLEEP_TIME);
    }
    return 0;
}