
#include "periph/gpio.h"
#include "ztimer.h"
#include "dht.h"

#define DHT_PIN             GPIO_PIN(0, 18)
#define LED_PIN             GPIO_PIN(0, 21)
#define TEMP_THRESHOLD      30
#define SLEEP_INTERVAL      5000000

static void _gpio_toggle(gpio_t pin)
{
    gpio_set(pin);
    ztimer_sleep(ZTIMER_MSEC, 100);
    gpio_clear(pin);
}

int main(void)
{
    dht_t dht;
    int16_t temp, hum;

    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    dht_init(&dht, DHT_TYPE_DHT11, DHT_PIN);

    while (1) {
        if (dht_read(&dht, &temp, &hum) == DHT_OK) {
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        ztimer_sleep(ZTIMER_USEC, SLEEP_INTERVAL);
    }

    return 0;
}
