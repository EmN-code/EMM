#include "timex.h"
#include "timer_arch.h"
#include "thread.h"
#include "periph/gpio.h"

// Define the LED GPIO pin (use an appropriate pin number for your ESP32 board)
#define LED_PIN GPIO_PIN(0, 2)

void timer_callback(void *arg) {
    // Toggle LED state
    static int state = 0;
    if (state) {
        gpio_clear(LED_PIN);
    } else {
        gpio_set(LED_PIN);
    }
    state = !state;
}

int main(void) {
    // Initialize the GPIO pin for the LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize a periodic timer that calls the timer_callback function every second
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        timer_callback(NULL);
        xtimer_periodic_wakeup(&last_wakeup, 1 * US_PER_SEC);
    }

    return 0;
}