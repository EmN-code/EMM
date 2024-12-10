
#include <stdio.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN             GPIO_PIN(0, 12)  // GPIO12 on ESP32
#define LED_PIN             GPIO_PIN(0, 2)   // GPIO2 on ESP32
#define TEMP_THRESHOLD      30               // Temperature threshold in Celsius
#define INTERVAL            5                // Interval in seconds

dht_t dev;

void temperature_callback(void *arg) {
    (void)arg;
    int16_t temp, hum;
    if (dht_read(&dev, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
               temp / 10, temp % 10, hum / 10, hum % 10);
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
    // Initialize the DHT11 sensor
    if (dht_init(&dev, DHT_PIN) == DHT_OK) {
        puts("DHT11 sensor initialized");
    } else {
        puts("Failed to initialize DHT11 sensor");
        return 1;
    }

    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Set up the timer for periodic reading
    xtimer_t timer;
    timer.callback = temperature_callback;
    timer.arg = NULL;
    xtimer_set(&timer, INTERVAL * 1000 * 1000);

    // Main loop
    while (1) {
        xtimer_usleep(1000000);  // Sleep for 1 second
    }

    return 0;
}
