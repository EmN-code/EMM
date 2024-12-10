
#include "xtimer.h"
#include "dht11.h"
#include "periph/gpio.h"

#define DHT11_GPIO_PIN GPIO_PIN(PORT_A, 0)
#define RGB_LED_PIN GPIO_PIN(PORT_A, 1)

void rgb_led_set(uint8_t colour) {
    gpio_write(RGB_LED_PIN, colour);
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();

    // Initialize DHT11 sensor
    dht11_init(DHT11_GPIO_PIN);

    // Initialize RGB LED
    gpio_init(RGB_LED_PIN, GPIO_OUT);

    while (1) {
        // Read DHT11 sensor
        int16_t temp, hum;
        dht11_read(&temp, &hum);

        // Turn on RGB LED
        rgb_led_set(1);

        // Wait for 5 seconds
        xtimer_periodic_wakeup(&last_wakeup, 5 * 1000 * 1000);

        // Turn off RGB LED
        rgb_led_set(0);
    }

    return 0;
}
