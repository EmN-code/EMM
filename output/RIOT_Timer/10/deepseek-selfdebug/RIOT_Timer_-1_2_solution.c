
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 13)  // Assuming GPIO 13 on port 0 is connected to an LED

void toggle_led(void) {
    static int state = 0;
    gpio_write(LED_PIN, state);
    state = !state;
}

int main(void) {
    // Initialize the xtimer module
    xtimer_init();

    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Set up a timer to trigger every second
    xtimer_t timer;
    timer.target = timer.long_target = xtimer_now();
    while (1) {
        xtimer_set(&timer, 1000000);  // 1 second delay
        toggle_led();
    }

    return 0;
}
