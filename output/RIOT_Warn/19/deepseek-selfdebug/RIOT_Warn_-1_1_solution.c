
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"
#include <stdio.h>

#define DHT_PIN             GPIO_PIN(0, 18)  // GPIO pin for DHT11
#define LED_PIN             GPIO_PIN(0, 21)  // GPIO pin for LED
#define TEMP_THRESHOLD      25               // Temperature threshold in Celsius
#define SLEEP_INTERVAL      5                // Sleep interval in seconds

dht_t dht;

void init_dht(void) {
    dht_params_t params = {
        .pin = DHT_PIN,
        .type = DHT11
    };
    dht_init(&dht, &params);
}

void init_led(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_set(LED_PIN);  // Turn off LED initially
}

void read_dht_and_check_temp(void) {
    int16_t temp, hum;
    if (dht_read(&dht, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        if (temp / 10 > TEMP_THRESHOLD) {
            gpio_clear(LED_PIN);  // Turn on LED
        } else {
            gpio_set(LED_PIN);    // Turn off LED
        }
    } else {
        puts("Failed to read from DHT sensor");
    }
}

int main(void) {
    init_dht();
    init_led();

    while (1) {
        read_dht_and_check_temp();
        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}
