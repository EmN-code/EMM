
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN             GPIO_PIN(0, 18)  // GPIO pin for DHT11
#define LED_PIN             GPIO_PIN(0, 2)   // GPIO pin for LED
#define TEMP_THRESHOLD      25               // Temperature threshold in Celsius
#define INTERVAL_MS         5000000          // 5 seconds interval in microseconds

static void dht_read_handler(void) {
    int16_t temp, hum;
    if (dht_read(DHT_PIN, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        if (temp / 10 > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }
    } else {
        puts("Failed to read from DHT11 sensor");
    }
}

int main(void) {
    // Initialize GPIO pins
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_init(DHT_PIN, GPIO_IN_PU);  // Initialize DHT pin with pull-up resistor

    // Set up timer for periodic reading
    xtimer_t timer;
    xtimer_init(&timer);
    xtimer_set(&timer, INTERVAL_MS, dht_read_handler);

    // Main loop
    while (1) {
        xtimer_usleep(INTERVAL_MS);
    }

    return 0;
}
