
#include <stdio.h>
#include "thread.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define DHT11_SIMULATED_VALUE 50  // Simulated humidity value
#define RGB_LED_RED    GPIO_PIN(0, 17)
#define RGB_LED_GREEN  GPIO_PIN(0, 18)
#define RGB_LED_BLUE   GPIO_PIN(0, 19)

#define RGB_LED_WHITE   (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_MAGENTA (RGB_LED_RED | RGB_LED_BLUE)
#define RGB_LED_CYAN    (RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_YELLOW  (RGB_LED_RED | RGB_LED_GREEN)

void turn_on_rgb_led(uint32_t color) {
    gpio_set(RGB_LED_RED, color & RGB_LED_RED);
    gpio_set(RGB_LED_GREEN, color & RGB_LED_GREEN);
    gpio_set(RGB_LED_BLUE, color & RGB_LED_BLUE);
}

void turn_off_rgb_led(void) {
    gpio_clear(RGB_LED_RED);
    gpio_clear(RGB_LED_GREEN);
    gpio_clear(RGB_LED_BLUE);
}

int main(void) {
    // Initialize GPIO pins for RGB LED
    gpio_init(RGB_LED_RED, GPIO_OUT);
    gpio_init(RGB_LED_GREEN, GPIO_OUT);
    gpio_init(RGB_LED_BLUE, GPIO_OUT);

    while (1) {
        // Simulate reading DHT11 sensor
        int humidity = DHT11_SIMULATED_VALUE;
        printf("Simulated DHT11 Humidity: %d\n", humidity);

        // Turn on RGB LED based on the simulated humidity value
        if (humidity < 30) {
            turn_on_rgb_led(RGB_LED_CYAN);
        } else if (humidity < 60) {
            turn_on_rgb_led(RGB_LED_YELLOW);
        } else {
            turn_on_rgb_led(RGB_LED_RED);
        }

        // Wait for 5 seconds
        xtimer_sleep(5);

        // Turn off RGB LED
        turn_off_rgb_led();
    }

    return 0;
}
