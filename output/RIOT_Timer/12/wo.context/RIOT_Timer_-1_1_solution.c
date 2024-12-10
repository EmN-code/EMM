#include <timex.h>
#include <ztimer.h>
#include <periph/gpio.h>

#define BLINK_ALARM 1
#define LED_PIN GPIO_PIN(0, 2) // Using GPIO2 for LED

static void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the GPIO for the LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Set up the timer
    ztimer_ticks_t interval = ZTIMER_MS_TO_TICKS(ZTIMER_MSEC, 1000);
    ztimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Start the periodic timer
    int ret = ztimer_periodic_start(ZTIMER_MSEC, &timer, interval);
    if (ret < 0) {
        return 1; // Timer setting unsuccessful
    }

    while (1) {
        // Main loop, the timer will handle the LED toggling
    }

    return 0;
}