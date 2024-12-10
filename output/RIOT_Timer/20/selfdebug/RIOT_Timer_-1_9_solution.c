#include <ztimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <board.h>

#define LED_PIN GPIO_PIN(PORT_B, 5)

void timer_callback(void *arg) {
    (void)arg;
    // Toggle the LED state
    gpio_toggle(LED_PIN);
}

int main(void) {
    ztimer_t timer;

    // Initialize GPIO pin for LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Configure the timer
    ztimer_set_handler(ZTIMER_MSEC, &timer, timer_callback, NULL);
    ztimer_set(ZTIMER_MSEC, &timer, ztimer_usec_from_sec(ZTIMER_MSEC, 1));

    // Main loop: Idle here while the timer runs
    while (1) {
        ztimer_spin(ZTIMER_MSEC, 1000);
    }
    return 0;
}