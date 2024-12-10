
#include <stdio.h>
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN             GPIO_PIN(0, 18)  // GPIO pin for DHT11 sensor
#define LED_PIN             GPIO_PIN(0, 21)  // GPIO pin for LED
#define TEMP_THRESHOLD      30               // Temperature threshold in Celsius
#define INTERVAL_MS         5000             // Interval in milliseconds

static void read_dht11(void) {
    int16_t temp, hum;
    if (dht_read_temp(DHT_PIN, &temp, &hum, DHT_TYPE_DHT11) == 0) {
        printf("Temperature: %d C, Humidity: %d %%\n", temp, hum);
        if (temp > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);  // Turn on LED
        } else {
            gpio_clear(LED_PIN);  // Turn off LED
        }
    } else {
        puts("Failed to read from DHT11 sensor");
    }
}

int main(void) {
    // Initialize GPIO for LED
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);  // Ensure LED is off initially

    // Initialize DHT11 sensor
    if (dht_init(DHT_PIN, DHT_TYPE_DHT11) != 0) {
        puts("Initialization of DHT11 sensor failed");
        return 1;
    }

    while (1) {
        read_dht11();
        xtimer_sleep(INTERVAL_MS / 1000);  // Sleep for 5 seconds
    }

    return 0;
}
