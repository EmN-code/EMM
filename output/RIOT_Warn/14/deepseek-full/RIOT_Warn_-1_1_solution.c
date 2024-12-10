
#include <stdio.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN             GPIO_PIN(0, 18)  // GPIO pin for DHT11
#define LED_PIN             GPIO_PIN(0, 21)  // GPIO pin for LED
#define TEMP_THRESHOLD      30               // Temperature threshold in Celsius
#define INTERVAL            5                // Interval in seconds

static dht_t dht_dev;
static int16_t temperature, humidity;

void init_dht(void) {
    dht_params_t params = {
        .pin = DHT_PIN,
        .type = DHT11
    };
    dht_init(&dht_dev, &params);
}

void init_led(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);  // Ensure LED is off initially
}

void read_dht(void) {
    if (dht_read(&dht_dev, &temperature, &humidity) == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
               temperature / 10, temperature % 10, humidity / 10, humidity % 10);
    } else {
        puts("Failed to read from DHT sensor");
    }
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
        xtimer_sleep(INTERVAL);
    }

    return 0;
}
