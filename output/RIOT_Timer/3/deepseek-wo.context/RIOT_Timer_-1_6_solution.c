
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 13)  // Assuming GPIO 13 on ESP32

void timer_callback(void *arg) {
    (void)arg;  // Unused argument
    gpio_toggle(LED_PIN);  // Toggle the LED
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the xtimer (no arguments)
    xtimer_init();

    // Set up a periodic timer to trigger every second
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;
    xtimer_set(&timer, 1000000);  // 1 second interval

    return 0;
}
