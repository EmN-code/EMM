
#include <stdio.h>
#include <xtimer.h>
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN             GPIO_PIN(0, 17)  // GPIO17 on ESP32
#define LED_PIN             GPIO_PIN(0, 2)   // GPIO2 on ESP32
#define TEMP_THRESHOLD      30               // Temperature threshold in Celsius
#define INTERVAL            5                // Interval in seconds

static void read_dht11(void) {
    int16_t temp, hum;
    if (dht_read(DHT_TYPE_DHT11, DHT_PIN, &hum, &temp) == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
               temp / 10, temp % 10, hum / 10, hum % 10);

        if (temp / 10 > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);  // Turn on LED
        } else {
            gpio_clear(LED_PIN);  // Turn off LED
        }
    } else {
        puts("Failed to read from DHT11 sensor");
    }
}

int main(void) {
    // Initialize LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize DHT11 sensor
    if (dht_init(DHT_TYPE_DHT11, DHT_PIN) != DHT_OK) {
        puts("DHT11 sensor init failed");
        return 1;
    }

    while (1) {
        read_dht11();
        xtimer_sleep(INTERVAL);  // Wait for 5 seconds
    }

    return 0;
}
