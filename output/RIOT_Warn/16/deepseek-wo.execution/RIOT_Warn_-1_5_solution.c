
#include <stdio.h>
#include <unistd.h>
#include <periph/gpio.h>
#include <ztimer.h>
#include <stdbool.h>

#define DHT_PIN         GPIO_PIN(0, 18)
#define LED_PIN         GPIO_PIN(0, 21)
#define TEMP_THRESHOLD  30

static void _dht_init(void) {
    gpio_init(DHT_PIN, GPIO_OUT);
    gpio_set(DHT_PIN);
    ztimer_sleep(ZTIMER_MSEC, 1000);
    gpio_init(DHT_PIN, GPIO_IN);
}

static int _dht_read(int16_t *temp, int16_t *hum) {
    // Simulated DHT11 reading
    *temp = 25; // Simulated temperature value
    *hum = 60;  // Simulated humidity value
    return 0;   // Success
}

static void _led_init(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);
}

static void _led_toggle(void) {
    gpio_toggle(LED_PIN);
}

int main(void) {
    int16_t temperature, humidity;

    _dht_init();
    _led_init();

    while (1) {
        if (_dht_read(&temperature, &humidity) == 0) {
            printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
            if (temperature > TEMP_THRESHOLD) {
                _led_toggle();
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read DHT11 sensor\n");
        }

        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}
