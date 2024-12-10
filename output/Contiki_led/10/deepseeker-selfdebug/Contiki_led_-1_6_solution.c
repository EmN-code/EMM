
#include "dht11_sensor.h"
#include "gpio-hal.h"
#include "xtimer.h"

#define RGB_LED_PIN GPIO_PIN(PORT_A, 5)

void turn_on_rgb_led(void) {
    gpio_hal_arch_pin_set_output(RGB_LED_PIN);
    gpio_hal_arch_pin_set(RGB_LED_PIN);
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        // Read DHT11 sensor
        int16_t temperature, humidity;
        dht11_sensor.value(&dht11_sensor, DHT11_VALUE_TEMPERATURE_INTEGER, &temperature);
        dht11_sensor.value(&dht11_sensor, DHT11_VALUE_HUMIDITY_INTEGER, &humidity);

        // Turn on RGB LED
        turn_on_rgb_led();

        // Wait for 5 seconds
        xtimer_periodic_wakeup(&last_wakeup, 5 * CLOCK_SECOND);
    }

    return 0;
}
