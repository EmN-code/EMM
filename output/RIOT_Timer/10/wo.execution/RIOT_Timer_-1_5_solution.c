#include "xtimer.h"
#include "periph/gpio.h"

#define TIMER_INTERVAL (1U * US_PER_SEC)
#define LED_PIN GPIO_PIN(PORT_A, 0)

static void timer_callback(void *arg) {
    (void)arg;
    // Toggle LED state
    gpio_toggle(LED_PIN);
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    ztimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Initialize and set the timer
    xtimer_set(&timer, TIMER_INTERVAL);

    // Keep the program running, typically in a wait loop
    while (1) {
        xtimer_usleep(TIMER_INTERVAL);
    }

    return 0;
}