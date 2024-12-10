
#include "periph/gpio.h"
#include "periph/timer.h"
#include "dht.h"
#include "xtimer.h"
#include <stdio.h>

#define DHT_PIN             GPIO_PIN(0, 12)  // GPIO12 on ESP32
#define LED_PIN             GPIO_PIN(0, 2)   // GPIO2 on ESP32
#define TEMP_THRESHOLD      30               // Temperature threshold in Celsius
#define INTERVAL            5                // Interval in seconds

void timer_callback(void *arg) {
    (void)arg;
    printf("Timer callback triggered\n");
}

int main(void) {
    dht_t dev;
    int16_t temp, hum;
    gpio_t led = LED_PIN;

    // Initialize the LED pin as output
    gpio_init(led, GPIO_OUT);
    gpio_set(led);  // Turn off the LED initially

    // Initialize the DHT11 sensor
    if (dht_init(&dev, DHT_PIN) == DHT_OK) {
        printf("DHT11 sensor initialized successfully\n");
    } else {
        printf("Failed to initialize DHT11 sensor\n");
        return 1;
    }

    // Set up a timer for periodic readings
    timer_t timer;
    timer_init(timer, 1000000, timer_callback, NULL);
    timer_set(timer, INTERVAL * 1000000);  // 5 seconds interval

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);

            // Check if temperature exceeds the threshold
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_clear(led);  // Turn on the LED
            } else {
                gpio_set(led);    // Turn off the LED
            }
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }

        // Wait for the next interval
        xtimer_sleep(INTERVAL);
    }

    return 0;
}
