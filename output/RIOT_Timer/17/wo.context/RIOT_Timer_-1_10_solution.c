#include <ztimer.h>
#include <xtimer.h>
#include <periph/gpio.h>

#define TIMER_INTERVAL   (1U * US_PER_SEC)  // Timer interval of 1 second
#define LED_PIN          GPIO_PIN(0, 2)    // Assuming LED is connected to pin 2 of PORT 0

static void timer_callback(void *arg) {
    (void)arg;
    gpio_set(LED_PIN);  // Turn on LED
}

int main(void) {
    // Initialize GPIO
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize timer
    ztimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Set timer initially
    xtimer_set(&timer, TIMER_INTERVAL);

    while (1) {
        // Spin until the next interval
        ztimer_spin_until(ZTIMER_MSEC, ztimer_now(ZTIMER_MSEC) + 1000);
    }

    return 0;
}