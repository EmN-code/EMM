
#include <stdio.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN             GPIO_PIN(0, 18)  // GPIO pin for DHT11
#define LED_PIN             GPIO_PIN(0, 21)  // GPIO pin for LED
#define TEMP_THRESHOLD      25              // Temperature threshold in Celsius
#define INTERVAL            5               // Interval in seconds

dht_t dht;
dht_params_t dht_params = { .pin = DHT_PIN };

void read_dht(void) {
    int16_t temp, hum;
    if (dht_read(&dht, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        if (temp / 10 > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);  // Turn on LED
        } else {
            gpio_clear(LED_PIN);  // Turn off LED
        }
    } else {
        puts("Failed to read from DHT sensor");
    }
}

int main(void) {
    // Initialize GPIO for LED
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);  // Ensure LED is off initially

    // Initialize DHT11 sensor
    if (dht_init(&dht, &dht_params) != DHT_OK) {
        puts("DHT11 initialization failed");
        return 1;
    }

    while (1) {
        read_dht();
        xtimer_sleep(INTERVAL);  // Wait for 5 seconds
    }

    return 0;
}