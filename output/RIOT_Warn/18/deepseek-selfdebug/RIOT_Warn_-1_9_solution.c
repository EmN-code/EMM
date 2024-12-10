
#include "periph/gpio.h"
#include "periph/timer.h"
#include "dht.h"
#include "xtimer.h"
#include <stdio.h>

#define DHT_PIN             GPIO_PIN(0, 12)  // GPIO12 on ESP32
#define LED_PIN             GPIO_PIN(0, 2)   // GPIO2 on ESP32
#define TEMP_THRESHOLD      30               // Temperature threshold in Celsius
#define INTERVAL            5                // Interval in seconds

dht_t dev;

void read_dht(void) {
    int16_t temp, hum;
    if (dht_read(&dev, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        if (temp / 10 > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }
    } else {
        puts("Failed to read from DHT sensor");
    }
}

int main(void) {
    // Initialize GPIO for LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize DHT sensor
    if (dht_init(&dev, DHT_PIN) == DHT_OK) {
        puts("DHT sensor initialized");
    } else {
        puts("Failed to initialize DHT sensor");
        return 1;
    }

    // Set up periodic timer for reading DHT sensor every 5 seconds
    xtimer_t timer;
    timer.callback = read_dht;
    timer.arg = NULL;
    xtimer_set(&timer, INTERVAL * 1000 * 1000);

    // Main loop
    while (1) {
        xtimer_usleep(1000000);  // Sleep for 1 second
    }

    return 0;
}
