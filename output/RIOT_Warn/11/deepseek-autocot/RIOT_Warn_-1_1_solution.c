
#include <stdio.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN             GPIO_PIN(0, 18)  // GPIO pin for DHT11
#define LED_PIN             GPIO_PIN(0, 21)  // GPIO pin for LED
#define TEMP_THRESHOLD      25               // Temperature threshold in Celsius
#define INTERVAL            5                // Interval in seconds

static dht_t dht11;
static int16_t temperature, humidity;

void read_dht11(void) {
    if (dht_read(&dht11, &temperature, &humidity) != DHT_OK) {
        puts("Failed to read from DHT11 sensor");
        return;
    }
    printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
           temperature / 10, temperature % 10, humidity / 10, humidity % 10);
}

void check_temperature(void) {
    if (temperature / 10 > TEMP_THRESHOLD) {
        gpio_set(LED_PIN);
        puts("Temperature exceeds threshold! LED ON");
    } else {
        gpio_clear(LED_PIN);
        puts("Temperature within threshold. LED OFF");
    }
}

int main(void) {
    // Initialize DHT11 sensor
    dht_params_t dht_params = {
        .pin = DHT_PIN,
        .type = DHT11
    };
    dht_init(&dht11, &dht_params);

    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    while (1) {
        read_dht11();
        check_temperature();
        xtimer_sleep(INTERVAL);
    }

    return 0;
}
