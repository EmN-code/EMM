
#include "xtimer.h"
#include "dht11_sensor.h"
#include "periph/gpio.h"

#define DHT11_GPIO_PIN GPIO_PIN(0, 15)
#define RGB_LED_PIN GPIO_PIN(0, 16)

void turn_on_rgb_led(void) {
    gpio_set(RGB_LED_PIN);
}

int main(void) {
    dht11_init(DHT11_GPIO_PIN);
    gpio_init(RGB_LED_PIN, GPIO_OUT);

    while (1) {
        int status = dht11_read();
        if (status == DHT11_STATUS_OKAY) {
            turn_on_rgb_led();
        }
        xtimer_sleep(5);
    }

    return 0;
}
