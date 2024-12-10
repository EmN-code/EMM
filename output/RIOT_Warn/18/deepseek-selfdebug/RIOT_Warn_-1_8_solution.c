
#include <stdio.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN             GPIO_PIN(0, 18)  // GPIO pin for DHT11
#define LED_PIN             GPIO_PIN(0, 21)  // GPIO pin for LED
#define TEMP_THRESHOLD      25               // Temperature threshold in Celsius
#define SLEEP_INTERVAL      5                // Sleep interval in seconds

static dht_t dht_dev;

void init_dht(void) {
    if (dht_init(&dht_dev, DHT_PIN) == DHT_OK) {
        printf("DHT11 sensor initialized successfully\n");
    } else {
        printf("Failed to initialize DHT11 sensor\n");
    }
}

void init_led(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_set(LED_PIN);  // Turn off LED initially
}

int main(void) {
    int16_t temp, hum;

    init_dht();
    init_led();

    while (1) {
        if (dht_read(&dht_dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);

            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_clear(LED_PIN);  // Turn on LED
            } else {
                gpio_set(LED_PIN);    // Turn off LED
            }
        } else {
            printf("Failed to read data from DHT11 sensor\n");
        }

        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}
