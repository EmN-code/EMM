
#include <stdio.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN             GPIO_PIN(0, 18)  // GPIO pin for DHT11
#define LED_PIN             GPIO_PIN(0, 2)   // GPIO pin for LED
#define TEMP_THRESHOLD      30               // Temperature threshold in Celsius
#define INTERVAL            5                // Interval in seconds

static dht_t dht_dev;
static int16_t temperature, humidity;

void read_dht11(void) {
    if (dht_read(&dht_dev, &temperature, &humidity) != DHT_OK) {
        puts("Failed to read from DHT11 sensor");
        return;
    }
    printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
           temperature / 10, temperature % 10, humidity / 10, humidity % 10);

    if (temperature / 10 > TEMP_THRESHOLD) {
        gpio_set(LED_PIN);
    } else {
        gpio_clear(LED_PIN);
    }
}

int main(void) {
    dht_params_t dht_params = {
        .pin = DHT_PIN,
        .type = DHT11
    };

    if (dht_init(&dht_dev, &dht_params) != DHT_OK) {
        puts("Failed to initialize DHT11 sensor");
        return 1;
    }

    if (gpio_init(LED_PIN, GPIO_OUT) < 0) {
        puts("Failed to initialize LED GPIO");
        return 1;
    }

    while (1) {
        read_dht11();
        xtimer_sleep(INTERVAL);
    }

    return 0;
}
