
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"
#include <stdio.h>

#define DHT_PIN             GPIO_PIN(0, 18)  // GPIO pin for DHT11
#define LED_PIN             GPIO_PIN(0, 2)   // GPIO pin for LED
#define TEMP_THRESHOLD      25               // Temperature threshold in Celsius
#define SLEEP_INTERVAL      5                // Sleep interval in seconds

static dht_t dht11;
static int16_t temperature, humidity;

void init_dht(void) {
    dht_params_t params = {
        .pin = DHT_PIN,
        .type = DHT11
    };
    dht_init(&dht11, &params);
}

void init_led(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);  // Ensure LED is off initially
}

void read_dht(void) {
    if (dht_read(&dht11, &temperature, &humidity) != DHT_OK) {
        puts("Failed to read from DHT11 sensor");
        return;
    }
    printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
           temperature / 10, temperature % 10, humidity / 10, humidity % 10);
}

void check_temperature(void) {
    if (temperature / 10 > TEMP_THRESHOLD) {
        gpio_set(LED_PIN);  // Turn on LED
    } else {
        gpio_clear(LED_PIN);  // Turn off LED
    }
}

int main(void) {
    init_dht();
    init_led();

    while (1) {
        read_dht();
        check_temperature();
        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}
