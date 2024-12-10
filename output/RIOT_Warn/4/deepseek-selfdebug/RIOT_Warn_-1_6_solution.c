
#include <stdio.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN             GPIO_PIN(0, 18)  // GPIO pin for DHT11
#define LED_PIN             GPIO_PIN(0, 21)  // GPIO pin for LED
#define TEMP_THRESHOLD      30               // Temperature threshold in Celsius
#define INTERVAL_MS         5000000          // 5 seconds interval in microseconds

static dht_t dht_dev;

void init_dht(void) {
    dht_dev.pin = DHT_PIN;
    dht_init(&dht_dev);
}

void init_led(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);  // Ensure LED is off initially
}

void read_dht(void) {
    int16_t temp, hum;
    if (dht_read(&dht_dev, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        if (temp / 10 > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);  // Turn on LED if temperature exceeds threshold
        } else {
            gpio_clear(LED_PIN);  // Turn off LED otherwise
        }
    } else {
        puts("Failed to read from DHT sensor");
    }
}

int main(void) {
    init_dht();
    init_led();

    while (1) {
        read_dht();
        xtimer_usleep(INTERVAL_MS);
    }

    return 0;
}
