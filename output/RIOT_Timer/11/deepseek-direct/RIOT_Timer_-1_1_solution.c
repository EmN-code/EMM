
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 16)  // Assuming GPIO0_16 is connected to an LED on ESP32

void toggle_led(void) {
    static int state = 0;
    gpio_write(LED_PIN, state);
    state = !state;
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Set up a timer to trigger every second
    xtimer_t timer;
    xtimer_init(&timer);
    xtimer_set(&timer, XTIMER_TICKS(1000000), toggle_led);  // 1 second interval

    // Main loop to keep the program running
    while (1) {
        xtimer_usleep(1000000);  // Sleep for 1 second
    }

    return 0;
}
